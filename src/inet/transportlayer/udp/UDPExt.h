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

#ifndef __INET_UDPEXT_H
#define __INET_UDPEXT_H


#include "inet/transportlayer/contract/udp/UDPDataIndicationExt.h"
#include "inet/transportlayer/udp/UDP.h"

namespace inet {

/**
 * Implements the UDP protocol: encapsulates/decapsulates user data into/from UDP.
 *
 * More info in the NED file.
 */
class INET_API UDPExt : public UDP
{

  protected:

    // process UDP packets coming from IP
    //virtual void processUDPPacket(UDPPacket *udpPacket);

    virtual void sendUp(cPacket *payload, SockDesc *sd, const L3Address& srcAddr, ushort srcPort, const L3Address& destAddr, ushort destPort, int interfaceId, int ttl, unsigned char tos);

    virtual void saveInfo(cPacket *pkt);

  public:
    UDPExt();
    virtual ~UDPExt();

  private:
    double lastpow;
    double lastsnr;
    double lastper;
    double lastqPerc;
    double lastqAbs;
};

} // namespace inet

#endif // ifndef __INET_UDP_H

