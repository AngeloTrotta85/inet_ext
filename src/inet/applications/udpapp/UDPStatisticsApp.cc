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
#include <stdio.h>
#include <stdlib.h>

#include <applications/udpapp/UDPStatisticsApp.h>
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"

#include "inet/common/geometry/common/Coord.h"

#include "inet/routing/aodv/AODVRouting.h"
#include "inet/routing/gpsr/GPSR.h"
#include "inet/routing/dymo/DYMO.h"

#include "inet/networklayer/contract/IRoutingTable.h"

#include "inet/networklayer/ipv4/IPv4Datagram.h"

namespace inet {

Define_Module(UDPStatisticsApp);

UDPStatisticsApp::~UDPStatisticsApp() {
    cancelAndDelete(autoMsg);
    cancelAndDelete(statMsg);
}

void UDPStatisticsApp::initialize(int stage)
{
    UDPBasicBurst::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {

        pysicalDataHistorySize = par("pysicalDataHistorySize");
        maxListSizeVariances = par("maxListSizeVariances");
        statTime = par("statTime");
        startStatTime = par("startStatTime");
        onlyMyInfoStat = par("onlyMyInfoStat").boolValue();
        //fileStat = par("fileStat").stringValue();
        snprintf(fileStat, sizeof(fileStat), "%s", par("fileStat").stringValue());
        remove(fileStat);

        snprintf(fileStatHeader, sizeof(fileStatHeader), "%s", par("fileStatHeader").stringValue());
        remove(fileStatHeader);

        mob = check_and_cast<IMobility *>(getParentModule()->getSubmodule("mobility"));
        dcfMac = check_and_cast<ieee80211::DcfUpperMacExt *>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("mac")->getSubmodule("upperMac"));
        udpbb = check_and_cast<UDPBasicBurstExt *>(getParentModule()->getSubmodule("udpApp", 1));

        radioTransmitter = check_and_cast<physicallayer::Ieee80211TransmitterBase *>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("radio")->getSubmodule("transmitter"));


        autoMsg = new cMessage("updateForce");
        scheduleAt(simTime() + dblrand(), autoMsg);

        statMsg = new cMessage("statMessage");
        //scheduleAt(simTime() + statTime, statMsg);

        WATCH_MAP(neighbourood);
        WATCH_LIST(myLastVelLength);
        WATCH_LIST(myLastVelTheta);
        WATCH_LIST(myLastDegree);
    }
    else if (stage == INITSTAGE_LAST) {
        myAddr = L3AddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
        myAppAddr = this->getParentModule()->getIndex();
        EV << "My address is: " << myAddr << std::endl;

        printFileStatHeader();

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
    else if ((msg->isSelfMessage()) && (msg == statMsg)) {
        handleStatAutoMsg();
        scheduleAt(simTime() + statTime, statMsg);
    }
    else {
        UDPBasicBurst::handleMessage(msg);
    }
}

void UDPStatisticsApp::handleStatAutoMsg(void) {

    /*if (firstInfoUpdate) {
        fillMyInfo(myLastInfo);
        setVariancesMeans(&myLastInfo);
        calculateAllMeanNeighbourood(myLastInfo);

        firstInfoUpdate = false;
    }

    makeStat();

    //set last info
    fillMyInfo(myLastInfo);
    setVariancesMeans(&myLastInfo);
    calculateAllMeanNeighbourood(myLastInfo);*/
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
        EV_DEBUG << "PER: " << di->getPer() << std::endl;
        EV_DEBUG << "MAC queue Perc: " << di->getQueueMacPerc() << std::endl;
        EV_DEBUG << "MAC queue Abs: " << di->getQueueMacAbs() << std::endl;
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
        n->lastPerRcv.push_front(info->getPer());
        if ((int)n->lastPerRcv.size() > pysicalDataHistorySize) {
            n->lastPerRcv.pop_back();
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

    struct nodeinfo myInfo, nextInfo, neighbourInfo;

    if (firstInfoUpdate) {
        fillMyInfo(myLastInfo);
        setVariancesMeans(&myLastInfo);
        calculateAllMeanNeighbourood(myLastInfo);

        firstInfoUpdate = false;
    }

    fillMyInfo(myInfo);
    updateMyInfoVector(&myInfo);
    setVariancesMeans(&myInfo);
    calculateAllMeanNeighbourood(myInfo);

    payload->setMyInfo(myInfo);

    fillNextInfo(nextInfo);
    payload->setNextInfo(nextInfo);

    fillNeighInfo(neighbourInfo);

    payload->setNeighboursArraySize(neighbourood.size());
    int ii = 0;
    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *actual = &(it->second);
        payload->setNeighbours(ii++, actual->nodeInf);
    }

    if (simTime() > (SIMTIME_ZERO + startStatTime))
        makeStat(&myInfo, &nextInfo, &neighbourInfo);

    myLastInfo = myInfo;

    return payload;
}

IPv4Address UDPStatisticsApp::getNextHopAddress(void) {
    IPv4Address nextAdd = IPv4Address::UNSPECIFIED_ADDRESS;
    IRoutingTable *routingTable = dynamic_cast<IRoutingTable *>(this->getParentModule()->getSubmodule("routingTable"));

    if (dynamic_cast<AODVRouting *>(this->getParentModule()->getSubmodule("aodv"))) {
        //AODVRouting *aodv = check_and_cast<AODVRouting *>(this->getParentModule()->getSubmodule("aodv"));
        IRoute *route = routingTable->findBestMatchingRoute(udpbb->getDestAddr());

        if (route) {
            //route->getMetric();
            nextAdd = route->getNextHopAsGeneric().toIPv4();

            EV << "AODV Routing: next hop is " << nextAdd << endl;
        }
    }
    else if (dynamic_cast<GPSR *>(this->getParentModule()->getSubmodule("gpsr"))) {
        GPSR *gpsr = check_and_cast<GPSR *>(this->getParentModule()->getSubmodule("gpsr"));

        IPv4Datagram *dummyPkt = new IPv4Datagram("dummy");
        gpsr->setGpsrOptionOnNetworkDatagram(dummyPkt);
        L3Address nextHop = gpsr->findNextHop(dummyPkt, udpbb->getDestAddr());
        delete dummyPkt;

        if (!nextHop.isUnspecified()) {
            nextAdd = nextHop.toIPv4();

            EV << "GPSR Routing: next hop is " << nextAdd << endl;
        }
        else {
            EV << "GPSR Routing: NO ROUTE to " << udpbb->getDestAddr() << endl;
        }
    }
    else if (dynamic_cast<dymo::DYMO *>(this->getParentModule()->getSubmodule("dymo"))) {
        //AODVRouting *aodv = check_and_cast<AODVRouting *>(this->getParentModule()->getSubmodule("aodv"));
        IRoute *route = routingTable->findBestMatchingRoute(udpbb->getDestAddr());

        if (route) {
            //route->getMetric();
            nextAdd = route->getNextHopAsGeneric().toIPv4();

            EV << "DYMO Routing: next hop is " << nextAdd << endl;
        }
        else {
            EV << "DYMO Routing: NO ROUTE to " << udpbb->getDestAddr() << endl;
        }
    }
    return nextAdd;
}

double UDPStatisticsApp::getL3Metric(void) {
    double ris = NaN;
    IRoutingTable *routingTable = dynamic_cast<IRoutingTable *>(this->getParentModule()->getSubmodule("routingTable"));

    if (dynamic_cast<AODVRouting *>(this->getParentModule()->getSubmodule("aodv"))) {
        IRoute *route = routingTable->findBestMatchingRoute(udpbb->getDestAddr());

        if (route) {
            ris = route->getMetric();
            EV << "AODV Routing metric: " << ris << endl;
        }
        else {
            EV << "AODV Routing metric: NO metric to " << udpbb->getDestAddr() << endl;
        }
    }
    else if (dynamic_cast<GPSR *>(this->getParentModule()->getSubmodule("gpsr"))) {
        GPSR *gpsr = check_and_cast<GPSR *>(this->getParentModule()->getSubmodule("gpsr"));

        Coord destPos = gpsr->getNeighborPosition(udpbb->getDestAddr());

        if(!destPos.isNil() && !destPos.isUnspecified()) {
        //if (destPos != Coord::NIL) {
            ris = destPos.distance(mob->getCurrentPosition());

            if (std::isnan(ris)) ris = 0.0;  //check

            EV << "GPSR Routing metric: " << ris << endl;
        }
        else {
            EV << "GPSR Routing metric: NO metric to " << udpbb->getDestAddr() << endl;
        }
    }
    else if (dynamic_cast<dymo::DYMO *>(this->getParentModule()->getSubmodule("dymo"))) {
        IRoute *route = routingTable->findBestMatchingRoute(udpbb->getDestAddr());

        if (route) {
            ris = route->getMetric();

            EV << "DYMO Routing metric: " << ris << endl;
        }
        else {
            EV << "DYMO Routing metric: NO metric to " << udpbb->getDestAddr() << endl;
        }
    }
    return ris;
}

double UDPStatisticsApp::getDistanceNextHop(void) {
    //double ris = std::numeric_limits<double>::max();
    double ris = -1;
    IPv4Address nextAdd = getNextHopAddress();

    if (nextAdd != IPv4Address::UNSPECIFIED_ADDRESS) {
        if (neighbourood.count(nextAdd) != 0) {
            ris = neighbourood[nextAdd].nodeInf.pos.distance(mob->getCurrentPosition());
            //if (std::isinf(ris) || std::isnan(ris)) ris = std::numeric_limits<double>::max();
            if (std::isinf(ris) || std::isnan(ris)) ris = -1;
        }
    }

    return ris;
}

double UDPStatisticsApp::getWeightedMean(std::list<double> *l) {
    double i = 1;

    double w = 0;
    double sum = 0;
    double ris = 0;

    for (auto it = l->begin(); it != l->end(); it++) {
        double actW = (1.0/i);
        sum += (*it) * actW;
        w += actW;

        i++;
    }
    if (w > 0) {
        ris = sum / w;
    }
    return ris;
}


void UDPStatisticsApp::getVarianceMean(std::list<double> *l, double &mean, double &variance) {
    double sum = 0.0;
    double m = 0.0;
    double v = 0.0;
    double lsize = l->size();

    EV << "Calculating Mean and Variance of ";
    for (auto it = l->begin(); it != l->end(); it++) EV << *it << " ";
    EV << endl;

    for (auto it = l->begin(); it != l->end(); it++) {
        sum += (*it);
    }

    if (lsize == 1){
        m = sum;
    }
    else if (lsize > 1) {
        double sumV = 0.0;

        m = sum / lsize;

        for (auto it = l->begin(); it != l->end(); it++) {
            sumV += pow(((*it) - m), 2.0);
        }
        v = sumV / (lsize - 1.0);
    }

    mean = m;
    variance = v;

    EV << "Result: Mean = " << mean << " and Variance = " << variance << endl;

}


void UDPStatisticsApp::calculateNeighMeanPhy(double &pow, double &snr, double &per) {
    double sumPow, sumSnr, sumPer;
    sumPow = sumSnr = sumPer = 0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *actual = &(it->second);

        sumPow += getWeightedMean(&(actual->lastPowRcv));
        sumSnr += getWeightedMean(&(actual->lastSnrRcv));
        sumPer += getWeightedMean(&(actual->lastPerRcv));
    }

    if (neighbourood.size() > 0){
        snr = sumSnr / ((double) neighbourood.size());
        if (std::isinf(snr) || std::isnan(snr)) snr = 0.0;

        pow = sumPow / ((double) neighbourood.size());
        if (std::isinf(pow) || std::isnan(pow)) pow = 0.0;

        per = sumPer / ((double) neighbourood.size());
        if (std::isinf(per) || std::isnan(per)) per = 0.0;
    }
}

double UDPStatisticsApp::calculateNeighMeanSnr(void) {
    double pow, snr, per;
    calculateNeighMeanPhy(pow, snr, per);
    return snr;
}

double UDPStatisticsApp::calculateNeighMeanPow(void) {
    double pow, snr, per;
    calculateNeighMeanPhy(pow, snr, per);
    return pow;
}

double UDPStatisticsApp::calculateNeighMeanPer(void) {
    double pow, snr, per;
    calculateNeighMeanPhy(pow, snr, per);
    return per;
}

double UDPStatisticsApp::calcMeanNeighDistance(void) {
    double count = neighbourood.size();
    double ris = -1;
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
    double ris = NaN;
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
    double ris = NaN;
    IPv4Address nextAdd = getNextHopAddress();

    if (nextAdd != IPv4Address::UNSPECIFIED_ADDRESS) {
        if (neighbourood.count(nextAdd) != 0) {
            struct nodeinfo *act = &(neighbourood[nextAdd].nodeInf);
            ris = calcApproachingVal(mob->getCurrentPosition(), mob->getCurrentSpeed(), act->pos, act->velocity);
        }
    }

    return ris;

}

void UDPStatisticsApp::updateMyInfoVector(struct nodeinfo *info) {
    myLastVelLength.push_front(info->velLength);
    myLastVelTheta.push_front(info->velTheta);
    myLastDegree.push_front(info->nodeDegree);
    if ((int)myLastVelLength.size() > maxListSizeVariances) {
        myLastVelLength.pop_back();
        myLastVelTheta.pop_back();
        myLastDegree.pop_back();
    }
}

void UDPStatisticsApp::setVariancesMeans(struct nodeinfo *info) {
    double mean, variance;

    getVarianceMean(&myLastVelLength, mean, variance);
    info->velLengthMean = mean;
    info->velLengthVariance = variance;

    getVarianceMean(&myLastVelTheta, mean, variance);
    info->velThetaMean = mean;
    info->velThetaVariance = variance;

    getVarianceMean(&myLastDegree, mean, variance);
    info->nodeDegreeVariance = variance;
}

void UDPStatisticsApp::calculateAllMeanNeighbourood(struct nodeinfo &info) {
    double count = neighbourood.size();

    Coord sumVel = Coord::ZERO;

    std::pair<double, double> pairNextDist (0.0, 0.0);
    std::pair<double, double> pairNextApproach (0.0, 0.0);
    std::pair<double, double> pairDist (0.0, 0.0);
    std::pair<double, double> pairApproach (0.0, 0.0);
    std::pair<double, double> pairL3Metric (0.0, 0.0);

    double sumSnr, sumPow, sumPer, sumDeg, sumNextDist, sumDist, sumApp, sumNextApp, sumL3Metric;
    double sumVelT, sumVelTMean, sumVelTVar, sumVelL, sumVelLMean, sumVelLVar, sumMacQAbs, sumMacQPerc;
    double sumThMSec, sumThVSec, sumThMNum, sumThVNum, sumDelMSec, sumDelVSec, sumDelMNum, sumDelVNum, sumPdrSec, sumPdrNum;
    double sumDegreeVar, sumThSTrend, sumDelSTrend, sumPdrSTrend;
    sumSnr = sumPow = sumPer = sumDeg = sumNextDist = sumDist = sumApp = sumNextApp = sumL3Metric = 0.0;
    sumVelT = sumVelTMean = sumVelTVar = sumVelL = sumVelLMean = sumVelLVar = sumMacQAbs = sumMacQPerc = 0.0;
    sumThMSec = sumThVSec = sumThMNum = sumThVNum = sumDelMSec = sumDelVSec = sumDelMNum = sumDelVNum = sumPdrSec = sumPdrNum = 0.0;
    sumDegreeVar = sumThSTrend = sumDelSTrend = sumPdrSTrend = 0.0;

    for (auto it = neighbourood.begin(); it != neighbourood.end(); it++) {
        neigh_t *act = &(it->second);
        sumSnr += act->nodeInf.snrNeighbourood;
        sumPow += act->nodeInf.powNeighbourood;
        sumPer += act->nodeInf.perNeighbourood;
        sumDeg += act->nodeInf.nodeDegree;
        sumVel += act->nodeInf.velocity;

        sumNextDist += act->nodeInf.nextHopDistance;
        if (act->nodeInf.nextHopDistance >= 0.0) {pairNextDist.first += act->nodeInf.nextHopDistance; pairNextDist.second += 1;}
        sumNextApp += act->nodeInf.nextHopApproaching;
        if (!(std::isnan(act->nodeInf.nextHopApproaching))) {pairNextApproach.first += act->nodeInf.nextHopApproaching; pairNextApproach.second += 1;}

        sumDist += act->nodeInf.distance;
        if (act->nodeInf.distance >= 0.0) {pairDist.first += act->nodeInf.distance; pairDist.second += 1;}
        sumApp += act->nodeInf.approaching;
        if (!(std::isnan(act->nodeInf.approaching))) {pairApproach.first += act->nodeInf.approaching; pairApproach.second += 1;}

        sumL3Metric += act->nodeInf.l3Metric;
        if (!(std::isnan(act->nodeInf.l3Metric))) {pairL3Metric.first += act->nodeInf.l3Metric; pairL3Metric.second += 1;}

        sumVelT += act->nodeInf.velTheta;
        sumVelTMean += act->nodeInf.velThetaMean;
        sumVelTVar += act->nodeInf.velThetaVariance;
        sumVelL += act->nodeInf.velLength;
        sumVelLMean += act->nodeInf.velLengthMean;
        sumVelLVar += act->nodeInf.velLengthVariance;
        sumMacQAbs += act->nodeInf.queueMacSizeAbs;
        sumMacQPerc += act->nodeInf.queueMacSizePerc;
        sumThMSec += act->nodeInf.througputMeanSecWindow;
        sumThVSec += act->nodeInf.througputVarSecWindow;
        sumThMNum += act->nodeInf.througputMeanNumWindow;
        sumThVNum += act->nodeInf.througputVarNumWindow;
        sumDelMSec += act->nodeInf.delayMeanSecWindow;
        sumDelVSec += act->nodeInf.delayVarSecWindow;
        sumDelMNum += act->nodeInf.delayMeanNumWindow;
        sumDelVNum += act->nodeInf.delayVarNumWindow;
        sumPdrSec += act->nodeInf.pdrSecWindow;
        sumPdrNum += act->nodeInf.pdrNumWindow;
        sumDegreeVar += act->nodeInf.nodeDegreeVariance;
        sumThSTrend += act->nodeInf.througputMeanTrendSecWindow;
        sumDelSTrend += act->nodeInf.delayMeanTrendSecWindow;
        sumPdrSTrend += act->nodeInf.pdrTrendSecWindow;
    }

    if (count == 0) {
        info.meanNodeDegreeNeighbourood = 0.0;
        info.meanPowNeighbourood = 0.0;
        info.meanPerNeighbourood = 0.0;
        info.meanSnrNeighbourood = 0.0;
        info.meanVelocityNeighbourood = Coord::ZERO;
        //info.meanNextHopDistanceNeighbourood = 0.0;
        //info.meanNextHopApproachingNeighbourood = 0.0;
        //info.meanDistanceNeighbourood = std::numeric_limits<double>::max();
        //info.meanApproachingNeighbourood = 0.0;
        info.meanVelThetaNeighbourood = 0.0;
        info.meanVelThetaMeanNeighbourood = 0.0;
        info.meanVelThetaVarianceNeighbourood = 0.0;
        info.meanVelLengthNeighbourood = 0.0;
        info.meanVelLengthMeanNeighbourood = 0.0;
        info.meanVelLengthVarianceNeighbourood = 0.0;
        info.meanQueueMacSizeAbsNeighbourood = 0.0;
        info.meanQueueMacSizePercNeighbourood = 0.0;
        //info.meanL3MetricNeighbourood = 0.0;
        info.meanThrougputMeanNumWindowNeighbourood = 0.0;
        info.meanThrougputVarNumWindowNeighbourood = 0.0;
        info.meanThrougputMeanSecWindowNeighbourood = 0.0;
        info.meanThrougputVarSecWindowNeighbourood = 0.0;
        info.meanDelayMeanNumWindowNeighbourood = 0.0;
        info.meanDelayVarNumWindowNeighbourood = 0.0;
        info.meanDelayMeanSecWindowNeighbourood = 0.0;
        info.meanDelayVarSecWindowNeighbourood = 0.0;
        info.meanPdrNumWindowNeighbourood = 0.0;
        info.meanPdrSecWindowNeighbourood = 0.0;
        info.meanNodeDegreeVarianceNeighbourood = 0.0;
        info.meanThrougputMeanTrendSecWindowNeighbourood = 0.0;
        info.meanDelayMeanTrendSecWindowNeighbourood = 0.0;
        info.meanPdrTrendSecWindowNeighbourood = 0.0;
    }
    else {
        info.meanNodeDegreeNeighbourood = sumDeg / count;
        info.meanPowNeighbourood = sumPow / count;
        info.meanPerNeighbourood = sumPer / count;
        info.meanSnrNeighbourood = sumSnr / count;
        info.meanVelocityNeighbourood = sumVel / count;
        //info.meanNextHopDistanceNeighbourood = sumNextDist / count;
        //info.meanNextHopApproachingNeighbourood = sumNextApp / count;
        //info.meanDistanceNeighbourood = sumDist / count;
        //info.meanApproachingNeighbourood = sumApp / count;
        info.meanVelThetaNeighbourood = sumVelT / count;
        info.meanVelThetaMeanNeighbourood = sumVelTMean/ count;
        info.meanVelThetaVarianceNeighbourood = sumVelTVar / count;
        info.meanVelLengthNeighbourood = sumVelL / count;
        info.meanVelLengthMeanNeighbourood = sumVelLMean / count;
        info.meanVelLengthVarianceNeighbourood = sumVelLVar / count;
        info.meanQueueMacSizeAbsNeighbourood = sumMacQAbs / count;
        info.meanQueueMacSizePercNeighbourood = sumMacQPerc / count;
        //info.meanL3MetricNeighbourood = sumL3Metric / count;
        info.meanThrougputMeanNumWindowNeighbourood = sumThMNum / count;
        info.meanThrougputVarNumWindowNeighbourood = sumThVNum / count;
        info.meanThrougputMeanSecWindowNeighbourood = sumThMSec / count;
        info.meanThrougputVarSecWindowNeighbourood = sumThVSec / count;
        info.meanDelayMeanNumWindowNeighbourood = sumDelMNum / count;
        info.meanDelayVarNumWindowNeighbourood = sumDelVNum / count;
        info.meanDelayMeanSecWindowNeighbourood = sumDelMSec / count;
        info.meanDelayVarSecWindowNeighbourood = sumDelVSec / count;
        info.meanPdrNumWindowNeighbourood = sumPdrNum / count;
        info.meanPdrSecWindowNeighbourood = sumPdrSec / count;
        info.meanNodeDegreeVarianceNeighbourood = sumDegreeVar / count;
        info.meanThrougputMeanTrendSecWindowNeighbourood = sumThSTrend / count;
        info.meanDelayMeanTrendSecWindowNeighbourood = sumDelSTrend / count;
        info.meanPdrTrendSecWindowNeighbourood = sumPdrSTrend / count;
    }

    // pair section
    if (pairNextDist.second > 0) info.meanNextHopDistanceNeighbourood = pairNextDist.first / pairNextDist.second;
    else info.meanNextHopDistanceNeighbourood = -1;
    if (pairNextApproach.second > 0) info.meanNextHopApproachingNeighbourood = pairNextApproach.first / pairNextApproach.second;
    else info.meanNextHopApproachingNeighbourood = 0.0;

    if (pairDist.second > 0) info.meanDistanceNeighbourood = pairDist.first / pairDist.second;
    else info.meanDistanceNeighbourood = -1;
    if (pairApproach.second > 0) info.meanApproachingNeighbourood = pairApproach.first / pairApproach.second;
    else info.meanApproachingNeighbourood = 0.0;

    if (pairL3Metric.second > 0) info.meanL3MetricNeighbourood = pairL3Metric.first / pairL3Metric.second;
    else info.meanL3MetricNeighbourood = 0.0;
}

void UDPStatisticsApp::fillMyInfo(struct nodeinfo &info) {
    info.localAddr = myAddr;
    info.appAddr = myAppAddr;
    info.nodeDegree = neighbourood.size();
    info.snrNeighbourood = calculateNeighMeanSnr();
    info.powNeighbourood = calculateNeighMeanPow();
    info.perNeighbourood = calculateNeighMeanPer();
    info.pos = mob->getCurrentPosition();
    info.velocity = mob->getCurrentSpeed();
    info.distance = calcMeanNeighDistance();
    info.approaching = calcApproachingNeigh();
    info.velLength = mob->getCurrentSpeed().length();
    info.velTheta = mob->getCurrentSpeed().angle(Coord(1.0, 0.0));
    if (std::isinf(info.velTheta) || std::isnan(info.velTheta)) info.velTheta = 0;


    //MAC
    info.queueMacSizeAbs = dcfMac->getQueueAbs();
    info.queueMacSizePerc = dcfMac->getQueuePerc();

    //L3 metric
    info.l3Metric = getL3Metric();

    //App info
    info.througputMeanSecWindow = udpbb->getThroughputMeanSec();
    info.througputVarSecWindow = udpbb->getThroughputVarSec();
    info.througputMeanNumWindow = udpbb->getThroughputMeanNum();
    info.througputVarNumWindow = udpbb->getThroughputVarNum();
    info.througputMeanTrendSecWindow = info.througputMeanSecWindow - myLastInfo.througputMeanSecWindow;

    info.delayMeanSecWindow = udpbb->getDelayMeanSec();
    info.delayVarSecWindow = udpbb->getDelayVarSec();
    info.delayMeanNumWindow = udpbb->getDelayMeanNum();
    info.delayVarNumWindow = udpbb->getDelayVarNum();
    info.delayMeanTrendSecWindow = info.delayMeanSecWindow - myLastInfo.delayMeanTrendSecWindow;

    info.pdrSecWindow = udpbb->getPDRSec();
    info.pdrNumWindow = udpbb->getPDRNum();
    info.pdrTrendSecWindow = info.pdrSecWindow - myLastInfo.pdrSecWindow;

    //NextHop related info
    info.nextHopDistance = getDistanceNextHop();
    info.nextHopApproaching = calcNextApproaching();

}

void UDPStatisticsApp::fillNextInfo(struct nodeinfo &info) {
    IPv4Address nextAdd = getNextHopAddress();

    info.appAddr = -1;

    if (nextAdd != IPv4Address::UNSPECIFIED_ADDRESS) {
        if (neighbourood.count(nextAdd) != 0) {
            //struct nodeinfo *act = &(neighbourood[nextAdd].nodeInf);
            info = neighbourood[nextAdd].nodeInf;
        }
    }
}

void UDPStatisticsApp::fillNeighInfo(struct nodeinfo &info) {
    info.appAddr = -1;

    if (neighbourood.size() > 0) {
        int index = intrand(neighbourood.size());
        int actIdx = 0;
        for (auto it = neighbourood.begin(); it != neighbourood.end(); it++){
            if (actIdx == index) {
                info = it->second.nodeInf;
                return;
            }
            actIdx++;
        }
    }
}

void UDPStatisticsApp::makeStat(struct nodeinfo *myInfo, struct nodeinfo *nextInfo, struct nodeinfo *neighbourInfo) {
    if(myInfo->nodeDegree > 0) {
        FILE *file = fopen(fileStat, "a");
        if(file) {
            std::stringstream ss;

            ss << simTime() << ";";
            ss << mob->getConstraintAreaMax().x - mob->getConstraintAreaMin().x << ";";
            ss << mob->getConstraintAreaMax().y - mob->getConstraintAreaMin().y << ";";
            ss << (mob->getConstraintAreaMax().x - mob->getConstraintAreaMin().x)*(mob->getConstraintAreaMax().y - mob->getConstraintAreaMin().y) << ";";
            ss << this->getParentModule()->getVectorSize() << ";";
            ss << radioTransmitter->getPower().get() << ";";
            ss << udpbb->getMessageLength() << ";";
            ss << udpbb->getMessageLengthSigma() << ";";
            ss << udpbb->getSendInterval() << ";";
            ss << udpbb->getBurstVolume() << ";";

            printStreamInfo(ss, myInfo);

            if (!onlyMyInfoStat) {

                if (nextInfo->appAddr < 0){
                    ss << ";0;";
                }
                else {
                    ss << ";1;";
                }
                printStreamInfo(ss, nextInfo);

                if (neighbourInfo->appAddr < 0){
                    ss << ";0;";
                }
                else {
                    ss << ";1;";
                }
                printStreamInfo(ss, neighbourInfo);

            }
            ss << endl;


            fwrite(ss.str().c_str(), ss.str().size(), 1, file);
            fclose(file);
        }
        else {
            EV << "Opening file " << fileStat << " failed with error: " << strerror(errno) << endl;
        }
    }
}

void UDPStatisticsApp::printStreamInfo(std::ostream& os, struct nodeinfo *i) {
    os <<
            i->velocity.x << ";" << i->velocity.y << ";" <<
            i->meanVelocityNeighbourood.x << ";" << i->meanVelocityNeighbourood.y << ";" <<
            i->velTheta << ";" << i->meanVelThetaNeighbourood << ";" <<
            i->velThetaMean << ";" << i->meanVelThetaMeanNeighbourood << ";" <<
            i->velThetaVariance << ";" << i->meanVelThetaVarianceNeighbourood << ";" <<
            i->velLength << ";" << i->meanVelLengthNeighbourood << ";" <<
            i->velLengthMean << ";" << i->meanVelLengthMeanNeighbourood << ";" <<
            i->velLengthVariance << ";" << i->meanVelLengthVarianceNeighbourood << ";" <<
            i->distance << ";" << i->meanDistanceNeighbourood << ";" <<
            (std::isnan(i->approaching) ? 0.0 : i->approaching) << ";" << i->meanApproachingNeighbourood << ";" <<
            i->nodeDegree << ";" << i->meanNodeDegreeNeighbourood << ";" <<
            i->nodeDegreeVariance << ";" << i->meanNodeDegreeVarianceNeighbourood << ";" <<
            i->snrNeighbourood << ";" << i->meanSnrNeighbourood << ";" <<
            i->powNeighbourood << ";" << i->meanPowNeighbourood << ";" <<
            i->perNeighbourood << ";" << i->meanPerNeighbourood << ";" <<
            i->queueMacSizeAbs << ";" << i->meanQueueMacSizeAbsNeighbourood << ";" <<
            i->queueMacSizePerc << ";" << i->meanQueueMacSizePercNeighbourood << ";" <<
            i->l3Metric << ";" << i->meanL3MetricNeighbourood << ";" <<
            i->througputMeanSecWindow << ";" << i->meanThrougputMeanSecWindowNeighbourood << ";" <<
            i->througputVarSecWindow << ";" << i->meanThrougputVarSecWindowNeighbourood << ";" <<
            i->througputMeanNumWindow << ";" << i->meanThrougputMeanNumWindowNeighbourood << ";" <<
            i->througputVarNumWindow << ";" << i->meanThrougputVarNumWindowNeighbourood << ";" <<
            i->througputMeanTrendSecWindow << ";" << i->meanThrougputMeanTrendSecWindowNeighbourood << ";" <<
            i->delayMeanSecWindow << ";" << i->meanDelayMeanSecWindowNeighbourood << ";" <<
            i->delayVarSecWindow << ";" << i->meanDelayVarSecWindowNeighbourood << ";" <<
            i->delayMeanNumWindow << ";" << i->meanDelayMeanNumWindowNeighbourood << ";" <<
            i->delayVarNumWindow << ";" << i->meanDelayVarNumWindowNeighbourood << ";" <<
            i->delayMeanTrendSecWindow << ";" << i->meanDelayMeanTrendSecWindowNeighbourood << ";" <<
            i->pdrSecWindow << ";" << i->meanPdrSecWindowNeighbourood << ";" <<
            i->pdrNumWindow << ";" << i->meanPdrNumWindowNeighbourood << ";" <<
            i->pdrTrendSecWindow << ";" << i->meanPdrTrendSecWindowNeighbourood << ";" <<
            i->nextHopDistance << ";" << i->meanNextHopDistanceNeighbourood << ";" <<
            (std::isnan(i->nextHopApproaching) ? 0.0 : i->nextHopApproaching) << ";" << i->meanNextHopApproachingNeighbourood <<
            "";
}

void UDPStatisticsApp::printFileStatHeader(void) {
    FILE *file = fopen(fileStatHeader, "w");
    if(file) {
        std::stringstream ss;
        int i0,i1;
        i0=0;
        i1=1;

        ss << i0++ << "\t" << i1++ << "\t" << "SimTime(s)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "AreaX(m)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "AreaY(m)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "AreaSquaredMeters(m2)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "NumberOfNodes" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "TransmittingPower(W)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "GeneratedMessageLength(B)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "GeneratedMessageLengthSigma(B)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "GeneratedPacketsSendInterval(s)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "GeneratedBurtsVolume(B/s)" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velocity.x" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velocity.y" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelocityNeighbourood.x" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelocityNeighbourood.y" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velTheta" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelThetaNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velThetaMean" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelThetaMeanNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velThetaVariance" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelThetaVarianceNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velLength" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelLengthNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velLengthMean" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelLengthMeanNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "velLengthVariance" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanVelLengthVarianceNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "distance" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDistanceNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "approaching" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanApproachingNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "nodeDegree" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanNodeDegreeNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "nodeDegreeVariance" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanNodeDegreeVarianceNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "snrNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanSnrNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "powNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanPowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "perNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanPerNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "queueMacSizeAbs" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanQueueMacSizeAbsNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "queueMacSizePerc" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanQueueMacSizePercNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "l3Metric" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanL3MetricNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "througputMeanSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanThrougputMeanSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "througputVarSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanThrougputVarSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "througputMeanNumWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanThrougputMeanNumWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "througputVarNumWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanThrougputVarNumWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "througputMeanTrendSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanThrougputMeanTrendSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "delayMeanSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDelayMeanSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "delayVarSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDelayVarSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "delayMeanNumWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDelayMeanNumWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "delayVarNumWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDelayVarNumWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "delayMeanTrendSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanDelayMeanTrendSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "pdrSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanPdrSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "pdrNumWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanPdrNumWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "pdrTrendSecWindow" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanPdrTrendSecWindowNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "nextHopDistance" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanNextHopDistanceNeighbourood" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "nextHopApproaching" << endl;
        ss << i0++ << "\t" << i1++ << "\t" << "meanNextHopApproachingNeighbourood" << endl;


        fwrite(ss.str().c_str(), ss.str().size(), 1, file);
        fclose(file);
    }
}


} /* namespace inet */
