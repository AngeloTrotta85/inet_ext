//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
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

#ifndef __INET_UDPBASICBURSTEXT_H
#define __INET_UDPBASICBURSTEXT_H


#include "inet/common/INETDefs.h"

#include "inet/applications/udpapp/UDPBasicBurst.h"

namespace inet {

/**
 * UDP application. See NED for more info.
 */
class INET_API UDPBasicBurstExt : public UDPBasicBurst
{

  protected:

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;

    // chooses random destination address
    virtual L3Address chooseDestAddr();
    virtual cPacket *createPacket();

  public:
    UDPBasicBurstExt() {};
    ~UDPBasicBurstExt() {};

    virtual L3Address getDestAddr(void);

  private:
  //  int destAddrIdx;
    L3Address myAddr;
    L3Address actualDestAddr;
};

} // namespace inet

#endif // ifndef __INET_UDPBASICBURSTEXT_H

