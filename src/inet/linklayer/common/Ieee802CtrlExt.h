//
// Copyright (C) 2011 Andras Varga
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

#ifndef __INET_IEEE802CTRLEXT_H
#define __INET_IEEE802CTRLEXT_H

#include "inet/common/INETDefs.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"

namespace inet {

/**
 * Represents a IEEE 802 control info. More info in the Ieee802Ctrl.msg file
 * (and the documentation generated from it).
 */
class INET_API Ieee802CtrlExt : public Ieee802Ctrl
{
protected:
    double rcvPow;
    double rcvSnr;
    double rcvPER;
    double queueMacPerc;
    double queueMacAbs;

  public:
    Ieee802CtrlExt() : Ieee802Ctrl() {}
    Ieee802CtrlExt(const Ieee802CtrlExt& other) : Ieee802Ctrl(other) {}
    Ieee802CtrlExt& operator=(const Ieee802CtrlExt& other) { Ieee802Ctrl::operator=(other); return *this; }

    virtual double getRcvPow() const { return rcvPow;    }
    virtual void setRcvPow(double rcvPow) { this->rcvPow = rcvPow; }

    virtual double getRcvSnr() const { return rcvSnr;  }
    virtual void setRcvSnr(double rcvSnr) { this->rcvSnr = rcvSnr; }

    double getQueueMacAbs() const { return queueMacAbs; }
    void setQueueMacAbs(double queueMacAbs) { this->queueMacAbs = queueMacAbs; }

    double getQueueMacPerc() const { return queueMacPerc; }
    void setQueueMacPerc(double queueMacPerc) { this->queueMacPerc = queueMacPerc; }

    double getRcvPER() const { return rcvPER; }
    void setRcvPER(double rcvPer) { rcvPER = rcvPer; }
};

} // namespace inet

#endif // ifndef __INET_IEEE802CTRL_H

