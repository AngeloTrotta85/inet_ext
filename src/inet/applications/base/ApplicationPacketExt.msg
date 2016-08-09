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

cplusplus {{
#include "inet/networklayer/common/L3Address.h"
#include "inet/common/geometry/common/Coord.h"
#include <list>

}}

namespace inet;

class noncobject L3Address;
class noncobject Coord;

struct nodeinfo
{
    L3Address localAddr;
    int appAddr;
    
    Coord pos;
    Coord velocity;
    Coord meanVelocityNeighbourood;
    
    double velTheta;
    double meanVelThetaNeighbourood;
    
    double velThetaVariance;
    double velThetaMean;
    double meanVelThetaVarianceNeighbourood;
    double meanVelThetaMeanNeighbourood;
    
    double velLength;
    double meanVelLengthNeighbourood;
    
    double velLengthVariance;
    double velLengthMean;
    double meanVelLengthVarianceNeighbourood;
    double meanVelLengthMeanNeighbourood;
    
    double distance;
    double meanDistanceNeighbourood;
    
    double approaching;
    double meanApproachingNeighbourood;
        
    int nodeDegree;
    double meanNodeDegreeNeighbourood;
    
    double nodeDegreeVariance;
    double meanNodeDegreeVarianceNeighbourood;
    
    double meanSnrNeighbourood;
    double snrNeighbourood;
	double meanPowNeighbourood;	
    double powNeighbourood;
	double meanPerNeighbourood;
    double perNeighbourood;
    
    // MAC info
    double queueMacSizeAbs;
    double meanQueueMacSizeAbsNeighbourood;
    double queueMacSizePerc;
    double meanQueueMacSizePercNeighbourood;
    
    // L3 metric
    double l3Metric;
    double meanL3MetricNeighbourood;
    
    // App info
    double througputMeanSecWindow;
    double througputVarSecWindow;
    double througputMeanNumWindow;
    double througputVarNumWindow;
    double meanThrougputMeanSecWindowNeighbourood;
    double meanThrougputVarSecWindowNeighbourood;
    double meanThrougputMeanNumWindowNeighbourood;
    double meanThrougputVarNumWindowNeighbourood;
    double througputMeanTrendSecWindow;
    double meanThrougputMeanTrendSecWindowNeighbourood;
    
    double delayMeanSecWindow;
    double delayVarSecWindow;
    double delayMeanNumWindow;
    double delayVarNumWindow;   
    double meanDelayMeanSecWindowNeighbourood;
    double meanDelayVarSecWindowNeighbourood;
    double meanDelayMeanNumWindowNeighbourood;
    double meanDelayVarNumWindowNeighbourood; 
    double delayMeanTrendSecWindow;
    double meanDelayMeanTrendSecWindowNeighbourood;   
    
    double pdrSecWindow;
    double pdrNumWindow;
    double meanPdrSecWindowNeighbourood;
    double meanPdrNumWindowNeighbourood;
    double pdrTrendSecWindow;
    double meanPdrTrendSecWindowNeighbourood;
    
    //NextHop related info    
    double nextHopDistance;
    double meanNextHopDistanceNeighbourood;
    double nextHopApproaching;
    double meanNextHopApproachingNeighbourood;
};

packet ApplicationPacketExt {
    long sequenceNumber;        
    
    nodeinfo myInfo;
    nodeinfo nextInfo;
    
    //int nodeDegree;
    //double meanSnrNeighbourood;
	//double meanPowNeighbourood;	
	
    nodeinfo neighbours[];
}
