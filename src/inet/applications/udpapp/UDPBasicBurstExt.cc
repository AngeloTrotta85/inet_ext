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


} // namespace inet

