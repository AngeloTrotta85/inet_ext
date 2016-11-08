//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package inet.applications.udpapp;

import inet.applications.udpapp.UDPBasicBurst;

simple UDPStatisticsApp extends UDPBasicBurst
{
    parameters:
        int pysicalDataHistorySize = default(5);
        int maxListSizeVariances = default(20);
        double statTime @unit(s) = default(2s);
        double startStatTime @unit(s) = default(30s);
        string fileStat = default ("/tmp/statTest.log");
        string fileStatHeader = default ("/tmp/statTestHeader.log");
        
        bool onlyMyInfoStat = default(true);
        
        @class(UDPStatisticsApp);
}
