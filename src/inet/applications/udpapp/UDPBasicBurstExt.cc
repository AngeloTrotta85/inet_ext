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
#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"
namespace inet {

Define_Module(UDPBasicBurstExt);


void UDPBasicBurstExt::initialize(int stage)
{
    UDPBasicBurst::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        myAddr = L3Address(IPv4Address::UNSPECIFIED_ADDRESS);
    }
    else if (stage == INITSTAGE_LAST) {
        myAddr = L3AddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
        EV << "My address is: " << myAddr << std::endl;

        WATCH(actualDestAddr);
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
    if (msg->getKind() == UDP_I_DATA) {
        // process incoming packet
        if (strncmp(msg->getName(), "UDPBasicAppExtData", strlen("UDPBasicAppExtData")) == 0) {
            handleIncomingPacket(check_and_cast<ApplicationPacket *>(msg));
        }
        else if (strncmp(msg->getName(), "UDPBasicAppExtAck", strlen("UDPBasicAppExtAck")) == 0) {
            handleIncomingAck(check_and_cast<ApplicationPacket *>(msg));
        }
    }
    UDPBasicBurst::handleMessageWhenUp(msg);
}

cPacket *UDPBasicBurstExt::createPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPBasicAppExtData-%d", counter++);
    long msgByteLength = messageLengthPar->longValue();
    ApplicationPacket *payload = new ApplicationPacket(msgName);
    payload->setByteLength(msgByteLength);
    payload->setSequenceNumber(numSent);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = numSent;

    return payload;
}

cPacket *UDPBasicBurstExt::createAckPacket(int index)
{
    char msgName[32];
    sprintf(msgName, "UDPBasicAppExtAck-%d", index);
    ApplicationPacket *payload = new ApplicationPacket(msgName);
    payload->setByteLength(32);

    return payload;
}

void UDPBasicBurstExt::handleIncomingPacket(ApplicationPacket *msg) {
    int counterRcv;
    sscanf(msg->getName(), "UDPBasicAppExtData-%d", &counterRcv);

    UDPDataIndication *di = check_and_cast<UDPDataIndication *>(msg->getControlInfo());

    cPacket *payload = createAckPacket(counterRcv);
    payload->setTimestamp();
    //emit(sentPkSignal, payload);

    socket.sendTo(payload, di->getSrcAddr(), di->getSrcPort());
}

void UDPBasicBurstExt::handleIncomingAck(ApplicationPacket *msg) {

}

} // namespace inet

