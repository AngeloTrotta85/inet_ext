//
// Copyright (C) 2006 Andras Varga
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

#ifndef __INET_IEEE80211MGMTADHOCEXT_H
#define __INET_IEEE80211MGMTADHOCEXT_H

#include "inet/common/INETDefs.h"

#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhoc.h"

namespace inet {

namespace ieee80211 {

/**
 * Used in 802.11 ad-hoc mode. See corresponding NED file for a detailed description.
 * This implementation ignores many details.
 *
 * @author Andras Varga
 */
class INET_API Ieee80211MgmtAdhocExt : public Ieee80211MgmtAdhoc
{
  protected:

    /** Utility method to decapsulate a data frame */
    virtual cPacket *decapsulate(Ieee80211DataFrame *frame);

};

} // namespace ieee80211

} // namespace inet

#endif // ifndef __INET_IEEE80211MGMTADHOCEXT_H

