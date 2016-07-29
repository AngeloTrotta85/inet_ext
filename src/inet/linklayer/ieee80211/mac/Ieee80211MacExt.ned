//
// Copyright (C) 2006 Andras Varga and Levente M�sz�ros
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
// along with this program; if not, see http://www.gnu.org/licenses/.
//
// Author: Andras Varga
//

package inet.linklayer.ieee80211.mac;

import inet.linklayer.ieee80211.IIeee80211Mac;

//
// Delegate some upperMac parameters to Ieee80211Mac, to be more compatible with the old MAC.
// NOTE: NOT ALL PARAMETERS OF THE OLD MAC CAN BE MAPPED.
//
module Ieee80211MacExt extends Ieee80211CompatibleMac
{
    parameters:
        
        upperMac.typename = "DcfUpperMacExt";
        
}
