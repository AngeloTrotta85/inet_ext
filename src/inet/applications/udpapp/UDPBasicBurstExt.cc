//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2007 Universidad de Málaga
// Copyright (C) 2011 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include <UDPBasicBurstExt.h>
#include "inet/networklayer/common/L3AddressResolver.h"

#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"
namespace inet {

Define_Module(UDPBasicBurstExt);


void UDPBasicBurstExt::initialize(int stage)
{
    UDPBasicBurst::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        myAddr = L3Address(IPv4Address::UNSPECIFIED_ADDRESS);

        maxStatSizeSec = par("maxStatSizeSec");
        maxStatSizeNum = par("maxStatSizeNum");
    }
    else if (stage == INITSTAGE_LAST) {
        myAddr = L3AddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
        EV << "My address is: " << myAddr << std::endl;

        WATCH(actualDestAddr);
        WATCH_LIST(transmissionsStatusSec);
        WATCH_LIST(transmissionsStatusNum);
    }
}


/*
L3Address UDPBasicBurstExt::chooseDestAddr()
{
    if (destAddresses.size() == 1)
        return destAddresses[0];

    if (destAddrIdx < 0) {
        destAddrIdx = getRNG(destAddrRNG)->intRand(destAddresses.size());
    }
    return destAddresses[destAddrIdx];
}
*/

double UDPBasicBurstExt::getMessageLength(void){
    double sumVal = 0.0;
    double countVal = 0.0;

    for (int i=0; i < 50; i++) {
        sumVal += messageLengthPar->longValue();
        countVal++;
    }
    return (sumVal/countVal);
};

double UDPBasicBurstExt::getMessageLengthSigma(void){
    double sumVal = 0.0;
    double countVal = 0.0;
    double mean = getMessageLength();

    for (int i=0; i < 50; i++) {
        sumVal += pow(messageLengthPar->longValue() - mean, 2);
        countVal++;
    }

    return sqrt(sumVal/(countVal-1.0));
};

double UDPBasicBurstExt::getSendInterval(void){
    double sumVal = 0.0;
    double countVal = 0.0;

    for (int i=0; i < 50; i++) {
        sumVal += sendIntervalPar->doubleValue();
        countVal++;
    }
    return (sumVal/countVal);
};

double UDPBasicBurstExt::getBurstVolume(void){
    return (getMessageLength()/getSendInterval());
}

L3Address UDPBasicBurstExt::getDestAddr(void) {
    return actualDestAddr;
}

L3Address UDPBasicBurstExt::chooseDestAddr()
{
    if (destAddresses.size() == 1){
        actualDestAddr = destAddresses[0];
        //return destAddresses[0];
    }
    else {

        int k = 0;

        for (int i = 0; i < 10; i++) {
            k = getRNG(destAddrRNG)->intRand(destAddresses.size());

            if (myAddr != destAddresses[k])
                break;
        }

        EV << myAddr << " --> Generating burst for " << destAddresses[k] << " - " << k << std::endl;

        actualDestAddr = destAddresses[k];

        //return destAddresses[k];
    }

    return actualDestAddr;
}

void UDPBasicBurstExt::handleMessageWhenUp(cMessage *msg)
{
    EV << "Received packet: " << msg->getName() << endl;
    if (msg->getKind() == UDP_I_DATA) {
        EV << "Received packet from network: " << msg->getName() << endl;
        // process incoming packet
        if (strncmp(msg->getName(), "UDPBasicAppExtData", strlen("UDPBasicAppExtData")) == 0) {
            handleIncomingPacket(check_and_cast<ApplicationPacketBurst *>(msg));
        }
        else if (strncmp(msg->getName(), "UDPBasicAppExtAck", strlen("UDPBasicAppExtAck")) == 0) {
            handleIncomingAck(check_and_cast<ApplicationPacketBurst *>(msg));
            delete msg;
            return;
        }
    }
    UDPBasicBurst::handleMessageWhenUp(msg);
}

cPacket *UDPBasicBurstExt::createPacket()
{
    char msgName[32];
    int newcounter = counter++;
    sprintf(msgName, "UDPBasicAppExtData-%d", newcounter);
    long msgByteLength = messageLengthPar->longValue();
    ApplicationPacketBurst *payload = new ApplicationPacketBurst(msgName);
    payload->setByteLength(msgByteLength);
    payload->setSequenceNumber(numSent);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = numSent;

    payload->setByteSent(msgByteLength);
    payload->setTimeSent(simTime());

    //update stats
    transmissionStatus_t newStatus;
    newStatus.pktCode = newcounter;
    newStatus.pktSizeByte = payload->getByteSent();
    newStatus.simtimeSent = newStatus.simtimeReceivedAck = payload->getTimeSent();
    newStatus.acknowledged = false;
    addToMapSec(&newStatus);
    addToMapNum(&newStatus);

    return payload;
}

cPacket *UDPBasicBurstExt::createAckPacket(ApplicationPacketBurst *msg2ack, int index)
{
    char msgName[32];
    sprintf(msgName, "UDPBasicAppExtAck-%d", index);
    ApplicationPacketBurst *payload = new ApplicationPacketBurst(msgName);
    payload->setByteLength(32);

    payload->setByteSent(msg2ack->getByteSent());
    payload->setTimeSent(msg2ack->getTimeSent());

    return payload;
}

void UDPBasicBurstExt::handleIncomingPacket(ApplicationPacketBurst *msg) {
    int counterRcv;
    sscanf(msg->getName(), "UDPBasicAppExtData-%d", &counterRcv);

    UDPDataIndication *di = check_and_cast<UDPDataIndication *>(msg->getControlInfo());

    cPacket *payload = createAckPacket(msg, counterRcv);
    payload->setTimestamp();
    socket.sendTo(payload, di->getSrcAddr(), di->getSrcPort());
}

void UDPBasicBurstExt::handleIncomingAck(ApplicationPacketBurst *msg) {
    int counterRcv;
    sscanf(msg->getName(), "UDPBasicAppExtAck-%d", &counterRcv);
    EV << "Received ACK for pkt " << counterRcv << endl;

    transmissionStatus_t newStatus;

    newStatus.pktCode = counterRcv;
    newStatus.pktSizeByte = msg->getByteSent();
    newStatus.simtimeSent = msg->getTimeSent();
    newStatus.simtimeReceivedAck = simTime();
    newStatus.acknowledged = true;

    addToMapSec(&newStatus);
    addToMapNum(&newStatus);
}

void UDPBasicBurstExt::addToMapSec(transmissionStatus_t *stat) {
    if (stat->acknowledged) {
        for (auto it = transmissionsStatusSec.begin(); it != transmissionsStatusSec.end(); it++) {
            if (it->pktCode == stat->pktCode) {
                *it = *stat;
                break;
            }
        }
    }
    else {
        transmissionsStatusSec.push_back(*stat);
    }
    //check
    if (transmissionsStatusSec.size() > 0) {
        transmissionStatus_t *statCheck = &(*transmissionsStatusSec.begin());
        while ((simTime() - statCheck->simtimeSent) > maxStatSizeSec) {
            transmissionsStatusSec.pop_front();

            if (transmissionsStatusSec.size() == 0) break;

            statCheck = &(*transmissionsStatusSec.begin());
        }
    }
}

void UDPBasicBurstExt::addToMapNum(transmissionStatus_t *stat) {
    if (stat->acknowledged) {
        for (auto it = transmissionsStatusNum.begin(); it != transmissionsStatusNum.end(); it++) {
            if (it->pktCode == stat->pktCode) {
                *it = *stat;
                break;
            }
        }
    }
    else {
        transmissionsStatusNum.push_back(*stat);
        if ((int)transmissionsStatusNum.size() > maxStatSizeNum) {
            transmissionsStatusNum.pop_front();
        }
    }
}

double UDPBasicBurstExt::getMean(std::list<double> *l) {
    double sum = 0.0;
    for (auto it = l->begin(); it != l->end(); it++) {
        sum += *it;
    }
    if (l->size() > 0) {
        return (sum / ((double) l->size()));
    }

    return 0;
}
double UDPBasicBurstExt::getVar(std::list<double> *l) {
    double sum = 0.0;
    double mean = getMean(l);

    for (auto it = l->begin(); it != l->end(); it++) {
        sum += pow(((*it) - mean), 2);
    }
    if (l->size() > 1) {
        return (sum / ((double) (l->size() - 1.0)));
    }

    return 0;

}

void UDPBasicBurstExt::getMeanVar_ThrougputDelayPDR(std::list<transmissionStatus_t> *list, double &throughput_val, double &throughput_var, double &delay_val, double &delay_var, double &pdr_val) {
    std::list<double> th;
    std::list<double> del;
    int countSucceed = 0;
    for (auto it = list->begin(); it != list->end(); it++) {
        if (it->acknowledged) {
            double del_val = (it->simtimeReceivedAck - it->simtimeSent).dbl() / 2.0;
            double th_val = ((double) it->pktSizeByte) / del_val;

            th.push_back(th_val);
            del.push_back(del_val);

            countSucceed++;
        }
    }

    throughput_val = getMean(&th);
    throughput_var = getVar(&th);
    delay_val = getMean(&del);
    delay_var = getVar(&del);
    pdr_val = 0.0;
    if (list->size() > 0) {
        pdr_val = ((double) countSucceed) / ((double) list->size());
    }
}

double UDPBasicBurstExt::getThroughputMeanSec(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusSec, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return throughput_val;
}
double UDPBasicBurstExt::getThroughputVarSec(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusSec, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return throughput_var;
}
double UDPBasicBurstExt::getThroughputMeanNum(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusNum, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return throughput_val;
}
double UDPBasicBurstExt::getThroughputVarNum(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusNum, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return throughput_var;
}

double UDPBasicBurstExt::getDelayMeanSec(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusSec, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return delay_val;
}
double UDPBasicBurstExt::getDelayVarSec(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusSec, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return delay_var;
}
double UDPBasicBurstExt::getDelayMeanNum(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusNum, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return delay_val;
}
double UDPBasicBurstExt::getDelayVarNum(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusNum, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return delay_var;
}

double UDPBasicBurstExt::getPDRSec(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusSec, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return pdr_val;
}
double UDPBasicBurstExt::getPDRNum(void) {
    double throughput_val, throughput_var, delay_val, delay_var, pdr_val;
    getMeanVar_ThrougputDelayPDR(&transmissionsStatusNum, throughput_val, throughput_var, delay_val, delay_var, pdr_val);
    return pdr_val;
}

} // namespace inet

