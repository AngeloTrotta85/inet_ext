//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004-2011 Andras Varga
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

#include <contract/ipv4/IPv4ControlInfoExt.h>
#include <UDPExt.h>

namespace inet {

Define_Module(UDPExt);

//--------
UDPExt::UDPExt()
{
}

UDPExt::~UDPExt()
{
}

void UDPExt::sendUp(cPacket *payload, SockDesc *sd, const L3Address& srcAddr, ushort srcPort, const L3Address& destAddr, ushort destPort, int interfaceId, int ttl, unsigned char tos) {

    EV_INFO << "Sending payload up to socket sockId=" << sd->sockId << "\n";

    EV_DEBUG << "UDPExt::sendUp sending POW: " << lastpow << " and SNR: " << lastsnr << std::endl;

    // send payload with UDPControlInfo up to the application
    UDPDataIndicationExt *udpCtrl = new UDPDataIndicationExt();
    udpCtrl->setSockId(sd->sockId);
    udpCtrl->setSrcAddr(srcAddr);
    udpCtrl->setDestAddr(destAddr);
    udpCtrl->setSrcPort(srcPort);
    udpCtrl->setDestPort(destPort);
    udpCtrl->setInterfaceId(interfaceId);
    udpCtrl->setTtl(ttl);
    udpCtrl->setTypeOfService(tos);
    udpCtrl->setPow(lastpow);
    udpCtrl->setSnr(lastsnr);
    payload->setControlInfo(udpCtrl);
    payload->setKind(UDP_I_DATA);

    emit(passedUpPkSignal, payload);
    send(payload, "appOut", sd->appGateIndex);
    numPassedUp++;
}

void UDPExt::saveInfo(cPacket *pkt) {
    //IPv4ControlInfo * ci = dynamic_cast<IPv4ControlInfo *>(ctrl);
    cObject *c = pkt->getControlInfo();
    EV_DEBUG << "UDPExt::saveInfo: " << c << std::endl;
    if (c) {
        /*EV_DEBUG << "detailedInfo Info: " << c->detailedInfo() << std::endl;
        EV_DEBUG << "getClassName Info: " << c->getClassName() << std::endl;
        EV_DEBUG << "getDescriptor Info: " << c->getDescriptor() << std::endl;
        EV_DEBUG << "getFullName Info: " << c->getFullName() << std::endl;
        EV_DEBUG << "getFullPath Info: " << c->getFullPath() << std::endl;
        EV_DEBUG << "getName Info: " << c->getName() << std::endl;
        EV_DEBUG << "getOwner Info: " << c->getOwner() << std::endl;
        EV_DEBUG << "info Info: " << c->info() << std::endl;*/

        IPv4ControlInfoExt *ip4ext = check_and_cast<IPv4ControlInfoExt *>(c);
        lastpow = ip4ext->getPow();
        lastsnr = ip4ext->getSnr();

        EV_DEBUG << "Saving POW: " << lastpow << " and SNR: " << lastsnr << std::endl;
    }
    else {
        EV_DEBUG << "NO CONTROL INFO: " << c << std::endl;
    }
};

}
