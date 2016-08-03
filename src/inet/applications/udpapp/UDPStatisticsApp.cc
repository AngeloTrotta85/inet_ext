//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <limits>       // std::numeric_limits

#include <applications/udpapp/UDPStatisticsApp.h>
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"

#include "inet/common/geometry/common/Coord.h"

namespace inet {

Define_Module(UDPStatisticsApp);

UDPStatisticsApp::~UDPStatisticsApp() {
    cancelAndDelete(autoMsg);
}

void UDPStatisticsApp::initialize(int stage)
{
    UDPBasicBurst::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        autoMsg = new cMessage("updateForce");
        scheduleAt(simTime() + dblrand(), autoMsg);

        pysicalDataHistorySize = par("pysicalDataHistorySize");

        mob = check_and_cast<IMobility *>(getParentModule()->getSubmodule("mobility"));

        WATCH_MAP(neighbourood);
    }
    else if (stage == INITSTAGE_LAST) {
        myAddr = L3AddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
        myAppAddr = this->getParentModule()->getIndex();
        EV << "My address is: " << myAddr << std::endl;

        this->getParentModule()->getDisplayString().setTagArg("t", 0, myAddr.str().c_str());
    }
}

void UDPStatisticsApp::finish() {
    UDPBasicBurst::finish();
}

void UDPStatisticsApp::handleMessage(cMessage *msg)
{
    if ((msg->isSelfMessage()) && (msg == autoMsg)) {
        handle100msAutoMsg();
        scheduleAt(simTime() + 0.1, autoMsg);
    }
    else {
        UDPBasicBurst::handleMessage(msg);
    }
}

void UDPStatisticsApp::handle100msAutoMsg(void) {
    updateNeighList();
}

void UDPStatisticsApp::processStart() {
    UDPBasicBurst::processStart();

    socket.setBroadcast(true);
}

void UDPStatisticsApp::processPacket(cPacket *pk)
{

    EV_DEBUG << "UDPStatisticsApp::processPacket START: " << pk << std::endl;
    if (pk->getKind() == UDP_I_ERROR) {
        EV_WARN << "UDP error received\n";
        delete pk;
        return;
    }

    if (pk->hasPar("sourceId") && pk->hasPar("msgId")) {
        // duplicate control
        int moduleId = (int)pk->par("sourceId");
        int msgId = (int)pk->par("msgId");
        auto it = sourceSequence.find(moduleId);
        if (it != sourceSequence.end()) {
            if (it->second >= msgId) {
                EV_DEBUG << "Out of order packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
                emit(outOfOrderPkSignal, pk);
                delete pk;
                numDuplicated++;
                return;
            }
            else
                it->second = msgId;
        }
        else
            sourceSequence[moduleId] = msgId;
    }

    if (delayLimit > 0) {
        if (simTime() - pk->getTimestamp() > delayLimit) {
            EV_DEBUG << "Old packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
            emit(dropPkSignal, pk);
            delete pk;
            numDeleted++;
            return;
        }
    }

    cObject *c = pk->getControlInfo();
    UDPDataIndicationExt *di = nullptr;
    EV_DEBUG << "UDPStatisticsApp::processPacket: " << c << std::endl;
    if (c) {
        EV_DEBUG << "detailedInfo Info: " << c->detailedInfo() << std::endl;
        EV_DEBUG << "getClassName Info: " << c->getClassName() << std::endl;
        EV_DEBUG << "getDescriptor Info: " << c->getDescriptor() << std::endl;
        EV_DEBUG << "getFullName Info: " << c->getFullName() << std::endl;
        EV_DEBUG << "getFullPath Info: " << c->getFullPath() << std::endl;
        EV_DEBUG << "getName Info: " << c->getName() << std::endl;
        EV_DEBUG << "getOwner Info: " << c->getOwner() << std::endl;
        EV_DEBUG << "info Info: " << c->info() << std::endl;

        di = check_and_cast<UDPDataIndicationExt *>(c);
        EV_DEBUG << "Pow: " << di->getPow() << std::endl;
        EV_DEBUG << "Snr: " << di->getSnr() << std::endl;
    }
    else {
        EV_DEBUG << "NO CONTROL INFO: " << c << std::endl;
    }

    manageReceivedPkt(pk, di);

    EV_INFO << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    emit(rcvdPkSignal, pk);
    numReceived++;
    delete pk;
}

void UDPStatisticsApp::manageReceivedPkt(cPacket *pk, UDPDataIndicationExt *info) {
    ApplicationPacketExt *pkt = check_and_cast<ApplicationPacketExt *>(pk);

    if (myAddr != pkt->getMyInfo().localAddr) {
        //EV_DEBUG << "PACKET Info" << std::endl;
        //EV_DEBUG << myAddr << " - addr Info: " << pkt->getLocalAddr() << std::endl;

        if (neighbourood.count(pkt->getMyInfo().localAddr) == 0) {
            neigh_t newN;
            neighbourood[pkt->getMyInfo().localAddr] = newN;
        }

        neigh_t *n = &neighbourood[pkt->getMyInfo().localAddr];
        n->nodeInf = pkt->getMyInfo();
        n->simtime = simTime();

        for (unsigned int i = 0; i < pkt->getNeighboursArraySize(); i++) {
            n->neigh.push_back(pkt->getNeighbours(i));
        }

        // get physical info
        n->lastPowRcv.push_front(info->getPow());
        if ((int)n->lastPowRcv.size() > pysicalDataHistorySize) {
            n->lastPowRcv.pop_back();
        }
        n->lastSnrRcv.push_front(info->getSnr());
        if ((int)n->lastSnrRcv.size() > pysicalDataHistorySize) {
            n->lastSnrRcv.pop_back();
        }
    }
}

void UDPStatisticsApp::updateNeighList(void) {
    bool toCheck;
    do {
        toCheck = false;
        for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
            neigh_t *actual = &(it->second);
            double sendInterval = sendIntervalPar->doubleValue();
            simtime_t passedTime = simTime() - actual->simtime;

            if (passedTime > (3.0 * sendInterval)) {
                neighbourood.erase(it);

                toCheck = true;
                break;
            }
        }
    } while (toCheck);
}

cPacket *UDPStatisticsApp::createPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPStatisticsApp-%d", counter++);
    long msgByteLength = messageLengthPar->longValue();
    ApplicationPacketExt *payload = new ApplicationPacketExt(msgName);
    payload->setByteLength(msgByteLength);
    payload->setSequenceNumber(numSent);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = numSent;

    struct nodeinfo myInfo, nextInfo;
    fillMyInfo(myInfo);
    payload->setMyInfo(myInfo);
    fillNextInfo(nextInfo);
    payload->setNextInfo(nextInfo);

    payload->setNeighboursArraySize(neighbourood.size());
    int ii = 0;
    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *actual = &(it->second);
        payload->setNeighbours(ii++, actual->nodeInf);
    }

    return payload;
}

IPv4Address UDPStatisticsApp::getNextHopAddress(void) {
    IPv4Address nextAdd = IPv4Address::UNSPECIFIED_ADDRESS;
    //TODO
    return nextAdd;
}

double UDPStatisticsApp::getDistanceNextHop(void) {
    double ris = std::numeric_limits<double>::max();
    IPv4Address nextAdd = getNextHopAddress();

    if (nextAdd != IPv4Address::UNSPECIFIED_ADDRESS) {
        if (neighbourood.count(nextAdd) != 0) {
            ris = neighbourood[nextAdd].nodeInf.pos.distance(mob->getCurrentPosition());
        }
    }

    return ris;
}

double UDPStatisticsApp::getMean(std::list<double> *l) {
    int i = 1;

    double w = 0;
    double sum = 0;
    double ris = 0;

    for (auto it = l->begin(); it != l->end(); it++) {
        sum += (*it) * (1.0/i);
        w += (1.0/i);

        i++;
    }
    if (w > 0) {
        ris = sum / w;
    }
    return ris;
}

void UDPStatisticsApp::calculateNeighMeanPhy(double &pow, double &snr) {
    double sumPow, sumSnr;
    sumPow = sumSnr = 0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *actual = &(it->second);

        sumPow += getMean(&(actual->lastPowRcv));
        sumSnr += getMean(&(actual->lastSnrRcv));
    }

    if (neighbourood.size() > 0){
        snr = sumSnr / ((double) neighbourood.size());
        pow = sumPow / ((double) neighbourood.size());
    }
}

double UDPStatisticsApp::calculateNeighMeanSnr(void) {
    double pow, snr;
    calculateNeighMeanPhy(pow, snr);
    return snr;
}

double UDPStatisticsApp::calculateNeighMeanPow(void) {
    double pow, snr;
    calculateNeighMeanPhy(pow, snr);
    return pow;
}

double UDPStatisticsApp::calcMeanNeighDistance(void) {
    double count = neighbourood.size();
    double ris = std::numeric_limits<double>::max();
    double sumD = 0.0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *act = &(it->second);
        sumD += act->nodeInf.pos.distance(mob->getCurrentPosition());
    }

    if (count > 0) {
        ris = sumD / count;
    }

    return ris;
}

double UDPStatisticsApp::calcApproachingVal(Coord posA, Coord velA, Coord posB, Coord velB) {
    double actDist = posA.distance(posB);
    Coord nextPosA = posA + velA;
    Coord nextPosB = posB + velB;
    double nextDist = nextPosA.distance(nextPosB);

    return (actDist - nextDist);
}

double UDPStatisticsApp::calcApproachingNeigh(void) {
    double count = neighbourood.size();
    double ris = 0.0;
    double sumA = 0.0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *act = &(it->second);
        sumA += calcApproachingVal(mob->getCurrentPosition(), mob->getCurrentSpeed(), act->nodeInf.pos, act->nodeInf.velocity);
    }

    if (count > 0) {
        ris = sumA / count;
    }

    return ris;

}

double UDPStatisticsApp::calcNextApproaching(void) {
    double ris = 0.0;
    IPv4Address nextAdd = getNextHopAddress();

    if (nextAdd != IPv4Address::UNSPECIFIED_ADDRESS) {
        if (neighbourood.count(nextAdd) != 0) {
            struct nodeinfo *act = &(neighbourood[nextAdd].nodeInf);
            ris = calcApproachingVal(mob->getCurrentPosition(), mob->getCurrentSpeed(), act->pos, act->velocity);
        }
    }

    return ris;

}

void UDPStatisticsApp::calculateAllMeanNeighbourood(struct nodeinfo &info) {
    double count = neighbourood.size();

    Coord sumVel = Coord::ZERO;
    double sumSnr, sumPow, sumDeg, sumNextDist, sumDist, sumApp, sumNextApp;
    sumSnr = sumPow = sumDeg = sumNextDist = sumDist = sumApp = sumNextApp = 0.0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *act = &(it->second);
        sumSnr += act->nodeInf.snrNeighbourood;
        sumPow += act->nodeInf.powNeighbourood;
        sumDeg += act->nodeInf.nodeDegree;
        sumVel += act->nodeInf.velocity;
        sumNextDist += act->nodeInf.nextHopDistance;
        sumDist += act->nodeInf.distance;
        sumApp += act->nodeInf.approaching;
        sumNextApp += act->nodeInf.nextHopApproaching;
    }

    if (count == 0) {
        info.meanNodeDegreeNeighbourood = 0.0;
        info.meanPowNeighbourood = 0.0;
        info.meanSnrNeighbourood = 0.0;
        info.meanVelocityNeighbourood = Coord::ZERO;
        info.meanNextHopDistanceNeighbourood = 0.0;
        info.meanDistanceNeighbourood = std::numeric_limits<double>::max();
        info.meanApproachingNeighbourood = 0.0;
        info.meanNextHopApproachingNeighbourood = 0.0;
    }
    else {
        info.meanNodeDegreeNeighbourood = sumDeg / count;
        info.meanPowNeighbourood = sumPow / count;
        info.meanSnrNeighbourood = sumSnr / count;
        info.meanVelocityNeighbourood = sumVel / count;
        info.meanNextHopDistanceNeighbourood = sumNextDist / count;
        info.meanDistanceNeighbourood = sumDist / count;
        info.meanApproachingNeighbourood = sumApp / count;
        info.meanNextHopApproachingNeighbourood = sumNextApp / count;
    }
}

void UDPStatisticsApp::fillMyInfo(struct nodeinfo &info) {
    info.localAddr = myAddr;
    info.appAddr = myAppAddr;
    info.nodeDegree = neighbourood.size();
    info.snrNeighbourood = calculateNeighMeanSnr();
    info.powNeighbourood = calculateNeighMeanPow();
    info.pos = mob->getCurrentPosition();
    info.velocity = mob->getCurrentSpeed();
    info.nextHopDistance = getDistanceNextHop();
    info.distance = calcMeanNeighDistance();
    info.approaching = calcApproachingNeigh();
    info.nextHopApproaching = calcNextApproaching();

    calculateAllMeanNeighbourood(info);
}

void UDPStatisticsApp::fillNextInfo(struct nodeinfo &info) {

}


} /* namespace inet */
