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

#ifndef __INET_IPV4EXT_H
#define __INET_IPV4EXT_H

#include "inet/common/INETDefs.h"

#include "inet/networklayer/ipv4/IPv4.h"


namespace inet {

/**
 * Implements the IPv4 protocol.
 */
class INET_API IPv4Ext : public IPv4
{

  protected:

    /**
     * Handle IPv4Datagram messages arriving from lower layer.
     * Decrements TTL, then invokes routePacket().
     */
    //virtual void handleIncomingDatagram(IPv4Datagram *datagram, const InterfaceEntry *fromIE);

    // called after PREROUTING Hook (used for reinject, too)
    virtual void preroutingFinish(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address nextHopAddr);

    /**
     * Decapsulate and return encapsulated packet after attaching IPv4ControlInfo.
     */
    virtual cPacket *decapsulate(IPv4Datagram *datagram);
};

} // namespace inet

#endif // ifndef __INET_IPV4_H

