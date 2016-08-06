//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include <common/Ieee802CtrlExt.h>
#include <contract/ipv4/IPv4ControlInfoExt.h>

#include <networklayer/ipv4/IPv4Ext.h>
#include <stdlib.h>
#include <string.h>

#include "inet/networklayer/arp/ipv4/ARPPacket_m.h"
#include "inet/networklayer/contract/IARP.h"
#include "inet/networklayer/ipv4/ICMPMessage_m.h"
#include "inet/networklayer/ipv4/IIPv4RoutingTable.h"
#include "inet/networklayer/common/IPSocket.h"
#include "inet/networklayer/ipv4/IPv4Datagram.h"
#include "inet/networklayer/ipv4/IPv4InterfaceData.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/LayeredProtocolBase.h"

#include "inet/networklayer/contract/ipv4/IPv4ControlInfoExt.h"

namespace inet {

Define_Module(IPv4Ext);

/*
void IPv4Ext::handleIncomingDatagram(IPv4Datagram *datagram, const InterfaceEntry *fromIE)
{
    ASSERT(datagram);
    ASSERT(fromIE);
    emit(LayeredProtocolBase::packetReceivedFromLowerSignal, datagram);

    //
    // "Prerouting"
    //

    // check for header biterror
    if (datagram->hasBitError()) {
        // probability of bit error in header = size of header / size of total message
        // (ignore bit error if in payload)
        double relativeHeaderLength = datagram->getHeaderLength() / (double)datagram->getByteLength();
        if (dblrand() <= relativeHeaderLength) {
            EV_WARN << "bit error found, sending ICMP_PARAMETER_PROBLEM\n";
            icmp->sendErrorMessage(datagram, fromIE->getInterfaceId(), ICMP_PARAMETER_PROBLEM, 0);
            return;
        }
    }

    EV_DETAIL << "Received datagram `" << datagram->getName() << "' with dest=" << datagram->getDestAddress() << "\n";

    const InterfaceEntry *destIE = nullptr;
    L3Address nextHop(IPv4Address::UNSPECIFIED_ADDRESS);
    if (datagramPreRoutingHook(datagram, fromIE, destIE, nextHop) == INetfilter::IHook::ACCEPT)
        preroutingFinish(datagram, fromIE, destIE, nextHop.toIPv4());
}
*/

void IPv4Ext::preroutingFinish(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address nextHopAddr)
{
    IPv4Address& destAddr = datagram->getDestAddress();

    // remove control info
    //delete datagram->removeControlInfo();

    // route packet

    if (fromIE->isLoopback()) {
        reassembleAndDeliver(datagram);
    }
    else if (destAddr.isMulticast()) {
        // check for local delivery
        // Note: multicast routers will receive IGMP datagrams even if their interface is not joined to the group
        if (fromIE->ipv4Data()->isMemberOfMulticastGroup(destAddr) ||
            (rt->isMulticastForwardingEnabled() && datagram->getTransportProtocol() == IP_PROT_IGMP))
            reassembleAndDeliver(datagram->dup());
        else
            EV_WARN << "Skip local delivery of multicast datagram (input interface not in multicast group)\n";

        // don't forward if IP forwarding is off, or if dest address is link-scope
        if (!rt->isMulticastForwardingEnabled()) {
            EV_WARN << "Skip forwarding of multicast datagram (forwarding disabled)\n";
            delete datagram;
        }
        else if (destAddr.isLinkLocalMulticast()) {
            EV_WARN << "Skip forwarding of multicast datagram (packet is link-local)\n";
            delete datagram;
        }
        else if (datagram->getTimeToLive() == 0) {
            EV_WARN << "Skip forwarding of multicast datagram (TTL reached 0)\n";
            delete datagram;
        }
        else
            forwardMulticastPacket(datagram, fromIE);
    }
    else {
        const InterfaceEntry *broadcastIE = nullptr;

        // check for local delivery; we must accept also packets coming from the interfaces that
        // do not yet have an IP address assigned. This happens during DHCP requests.
        if (rt->isLocalAddress(destAddr) || fromIE->ipv4Data()->getIPAddress().isUnspecified()) {
            reassembleAndDeliver(datagram);
        }
        else if (destAddr.isLimitedBroadcastAddress() || (broadcastIE = rt->findInterfaceByLocalBroadcastAddress(destAddr))) {
            // broadcast datagram on the target subnet if we are a router
            if (broadcastIE && fromIE != broadcastIE && rt->isForwardingEnabled())
                fragmentPostRouting(datagram->dup(), broadcastIE, IPv4Address::ALLONES_ADDRESS);

            EV_INFO << "Broadcast received\n";
            reassembleAndDeliver(datagram);
        }
        else if (!rt->isForwardingEnabled()) {
            EV_WARN << "forwarding off, dropping packet\n";
            numDropped++;
            delete datagram;
        }
        else
            routeUnicastPacket(datagram, fromIE, destIE, nextHopAddr);
    }
}

cPacket *IPv4Ext::decapsulate(IPv4Datagram *datagram)
{
    double snr, pow, per, qAbs, qPerc;
    snr = pow = per = qAbs = qPerc = 0;

    cObject *c = datagram->removeControlInfo();
    //EV_DEBUG << "IPv4Ext::decapsulate: " << c << std::endl;
    if (c) {
        /*EV_DEBUG << "detailedInfo Info: " << c->detailedInfo() << std::endl;
        EV_DEBUG << "getClassName Info: " << c->getClassName() << std::endl;
        EV_DEBUG << "getDescriptor Info: " << c->getDescriptor() << std::endl;
        EV_DEBUG << "getFullName Info: " << c->getFullName() << std::endl;
        EV_DEBUG << "getFullPath Info: " << c->getFullPath() << std::endl;
        EV_DEBUG << "getName Info: " << c->getName() << std::endl;
        EV_DEBUG << "getOwner Info: " << c->getOwner() << std::endl;
        EV_DEBUG << "info Info: " << c->info() << std::endl;*/

        inet::Ieee802CtrlExt *cExt = check_and_cast<inet::Ieee802CtrlExt *>(c);
        //EV_DEBUG << "Ieee802CtrlExt SNR: " << cExt->getRcvSnr() << std::endl;
        //EV_DEBUG << "Ieee802CtrlExt POW: " << cExt->getRcvPow() << std::endl;

        snr = cExt->getRcvSnr();
        pow = cExt->getRcvPow();
        per = cExt->getRcvPER();
        qAbs = cExt->getQueueMacAbs();
        qPerc = cExt->getQueueMacPerc();

        EV_DEBUG << "IPv4Ext::decapsulate Saving POW: " << pow << " and SNR: " << snr << std::endl;
    }
    else {
        EV_DEBUG << "NO CONTROL INFO: " << c << std::endl;
    }

    // decapsulate transport packet
    const InterfaceEntry *fromIE = getSourceInterfaceFrom(datagram);
    cPacket *packet = datagram->decapsulate();

    // create and fill in control info
    IPv4ControlInfoExt *controlInfo = new IPv4ControlInfoExt();
    controlInfo->setProtocol(datagram->getTransportProtocol());
    controlInfo->setSrcAddr(datagram->getSrcAddress());
    controlInfo->setDestAddr(datagram->getDestAddress());
    controlInfo->setTypeOfService(datagram->getTypeOfService());
    controlInfo->setInterfaceId(fromIE ? fromIE->getInterfaceId() : -1);
    controlInfo->setTimeToLive(datagram->getTimeToLive());

    controlInfo->setPow(pow);
    controlInfo->setSnr(snr);
    controlInfo->setPER(per);
    controlInfo->setQueueMacPerc(qPerc);
    controlInfo->setQueueMacAbs(qAbs);

    EV_DEBUG << "IPv4Ext::decapsulate Setting POW: " << pow << " and SNR: " << snr << std::endl;

    // original IPv4 datagram might be needed in upper layers to send back ICMP error message
    controlInfo->setOrigDatagram(datagram);

    // attach control info
    packet->setControlInfo(controlInfo);

    return packet;
}


} // namespace inet

