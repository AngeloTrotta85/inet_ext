//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_IPV4CONTROLINFOEXT_H
#define __INET_IPV4CONTROLINFOEXT_H

#include "inet/networklayer/contract/ipv4/IPv4ControlInfo.h"

namespace inet {

/**
 * Control information for sending/receiving packets over IPv4.
 *
 * See the IPv4ControlInfo.msg file for more info.
 */
class INET_API IPv4ControlInfoExt : public IPv4ControlInfo
{
  private:
    double snr;
    double pow;
    double rcvPER;
    double queueMacPerc;
    double queueMacAbs;

  public:
    IPv4ControlInfoExt() : IPv4ControlInfo() { pow = 0; snr = 0; };
    virtual ~IPv4ControlInfoExt() {};
    IPv4ControlInfoExt(const IPv4ControlInfoExt& other) : IPv4ControlInfo(other) {  }
    IPv4ControlInfoExt& operator=(const IPv4ControlInfoExt& other);
    virtual IPv4ControlInfoExt *dup() const override { return new IPv4ControlInfoExt(*this); }

    double getPow() const { return pow; }
    void setPow(double pow) { this->pow = pow; }

    double getSnr() const { return snr; }
    void setSnr(double snr) { this->snr = snr; }

    double getQueueMacAbs() const { return queueMacAbs; }
    void setQueueMacAbs(double queueMacAbs) { this->queueMacAbs = queueMacAbs; }

    double getQueueMacPerc() const { return queueMacPerc; }
    void setQueueMacPerc(double queueMacPerc) { this->queueMacPerc = queueMacPerc; }

    double getPER() const { return rcvPER; }
    void setPER(double rcvPer) { rcvPER = rcvPer; }
};

} // namespace inet

#endif // ifndef __INET_IPV4CONTROLINFO_H

