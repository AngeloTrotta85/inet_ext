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

#ifndef INET_APPLICATIONS_UDPAPP_UDPSTATISTICSAPP_H_
#define INET_APPLICATIONS_UDPAPP_UDPSTATISTICSAPP_H_

#include <vector>
#include <map>

#include "inet/common/INETDefs.h"


#include <ieee80211/mac/DcfUpperMacExt.h>

#include "inet/applications/udpapp/UDPBasicBurst.h"
#include "inet/applications/udpapp/UDPBasicBurstExt.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmitterBase.h"

#include "inet/transportlayer/contract/udp/UDPDataIndicationExt_m.h"
#include "inet/applications/base/ApplicationPacketExt_m.h"

#include "inet/mobility/contract/IMobility.h"

namespace inet {

class UDPStatisticsApp : public UDPBasicBurst {

public:

    typedef struct neighStruct{
        //L3Address addr;
        simtime_t simtime;

        // RECEIVED info
        struct nodeinfo nodeInf;
        std::list<struct nodeinfo> neigh;

        // local data. NOT SENT directly
        std::list<double> lastSnrRcv;
        std::list<double> lastPowRcv;
        std::list<double> lastPerRcv;

        friend std::ostream& operator << (std::ostream& os, neighStruct& n) {
            //os << "Addr: " << n.addr << " Degree: " << n.degree;// << " NeighList: " << neigh;

            os <<
                    //"Addr: " << n.nodeInf.localAddr <<
                    "[" << n.nodeInf.appAddr << "]" <<
                    " Deg: " << n.nodeInf.nodeDegree << "|" << n.nodeInf.nodeDegreeVariance << " [" << n.nodeInf.meanNodeDegreeNeighbourood << "|" << n.nodeInf.meanNodeDegreeVarianceNeighbourood << "]" <<
                    " SNR: " << n.nodeInf.snrNeighbourood << " [" << n.nodeInf.meanSnrNeighbourood << "]" <<
                    " Pow: " << n.nodeInf.powNeighbourood << " [" << n.nodeInf.meanPowNeighbourood << "]" <<
                    " Per: " << n.nodeInf.perNeighbourood << " [" << n.nodeInf.meanPerNeighbourood << "]" <<
                    " Pos: " << n.nodeInf.pos <<
                    " Vel: " << n.nodeInf.velocity << " [" << n.nodeInf.meanVelocityNeighbourood << "]" <<
                    " Dist: " << n.nodeInf.distance << " [" << n.nodeInf.meanDistanceNeighbourood << "]" <<
                    " NHDist: " << n.nodeInf.nextHopDistance << " [" << n.nodeInf.meanNextHopDistanceNeighbourood << "]" <<
                    " Appr: " << n.nodeInf.approaching << " [" << n.nodeInf.meanApproachingNeighbourood << "]" <<
                    " NHAppr: " << n.nodeInf.nextHopApproaching << " [" << n.nodeInf.meanNextHopApproachingNeighbourood << "]" <<
                    " VelT: " << n.nodeInf.velTheta << " [" << n.nodeInf.meanVelThetaNeighbourood << "]" <<
                    " VelTm: " << n.nodeInf.velThetaMean << " [" << n.nodeInf.meanVelThetaMeanNeighbourood << "]" <<
                    " VelTv: " << n.nodeInf.velThetaVariance << " [" << n.nodeInf.meanVelThetaVarianceNeighbourood << "]" <<
                    " VelL: " << n.nodeInf.velLength << " [" << n.nodeInf.meanVelLengthNeighbourood << "]" <<
                    " VelLm: " << n.nodeInf.velLengthMean << " [" << n.nodeInf.meanVelLengthMeanNeighbourood << "]" <<
                    " VelLv: " << n.nodeInf.velLengthVariance << " [" << n.nodeInf.meanVelLengthVarianceNeighbourood << "]" <<
                    " MacQAbs: " << n.nodeInf.queueMacSizeAbs << " [" << n.nodeInf.meanQueueMacSizeAbsNeighbourood << "]" <<
                    " MacQPerc: " << n.nodeInf.queueMacSizePerc << " [" << n.nodeInf.meanQueueMacSizePercNeighbourood << "]" <<
                    " ThrSec: " << n.nodeInf.througputMeanSecWindow << "|" << n.nodeInf.througputVarSecWindow << "|" << n.nodeInf.througputMeanTrendSecWindow << " [" << n.nodeInf.meanThrougputMeanSecWindowNeighbourood << "|" << n.nodeInf.meanThrougputVarSecWindowNeighbourood << "|" << n.nodeInf.meanThrougputMeanTrendSecWindowNeighbourood << "]" <<
                    " ThrNum: " << n.nodeInf.througputMeanNumWindow << "|" << n.nodeInf.througputVarNumWindow << " [" << n.nodeInf.meanThrougputMeanNumWindowNeighbourood << "|" << n.nodeInf.meanThrougputVarNumWindowNeighbourood << "]" <<
                    " DelaySec: " << n.nodeInf.delayMeanSecWindow << "|" << n.nodeInf.delayVarSecWindow << "|" << n.nodeInf.delayMeanTrendSecWindow << " [" << n.nodeInf.meanDelayMeanSecWindowNeighbourood << "|" << n.nodeInf.meanDelayVarSecWindowNeighbourood << "|" << n.nodeInf.meanDelayMeanTrendSecWindowNeighbourood << "]" <<
                    " DelayNum: " << n.nodeInf.delayMeanNumWindow << "|" << n.nodeInf.delayVarNumWindow << " [" << n.nodeInf.meanDelayMeanNumWindowNeighbourood << "|" << n.nodeInf.meanDelayVarNumWindowNeighbourood << "]" <<
                    " PDRSec: " << n.nodeInf.pdrSecWindow << "|" << n.nodeInf.pdrTrendSecWindow << " [" << n.nodeInf.meanPdrSecWindowNeighbourood << "|" << n.nodeInf.meanPdrTrendSecWindowNeighbourood << "]" <<
                    " PDRNum: " << n.nodeInf.pdrNumWindow << " [" << n.nodeInf.meanPdrNumWindowNeighbourood << "]" <<
                    "";

            return os;
        }
    } neigh_t;

protected:
    cMessage *autoMsg = nullptr;
    cMessage *statMsg = nullptr;

private:
  L3Address myAddr;
  int myAppAddr;
  std::map<L3Address, neigh_t> neighbourood;

  struct nodeinfo myLastInfo;
  bool firstInfoUpdate = true;

  // for variances
  std::list<double> myLastVelLength;
  std::list<double> myLastVelTheta;
  std::list<double> myLastDegree;

  //parameters
  int pysicalDataHistorySize;
  int maxListSizeVariances;
  double statTime;
  double startStatTime;
  char fileStat[256];
  char fileStatHeader[256];

  bool onlyMyInfoStat;

  IMobility *mob;
  ieee80211::DcfUpperMacExt *dcfMac;
  UDPBasicBurstExt *udpbb;
  physicallayer::Ieee80211TransmitterBase *radioTransmitter;

protected:
  virtual int numInitStages() const override { return NUM_INIT_STAGES; }
  virtual void initialize(int stage) override;
  virtual void handleMessage(cMessage *msg) override;
  virtual void finish() override;

  virtual void handle100msAutoMsg(void);
  virtual void handleStatAutoMsg(void);
  virtual void updateNeighList(void);

  virtual void processStart();

  virtual void processPacket(cPacket *msg);
  virtual cPacket *createPacket();

  virtual void manageReceivedPkt(cPacket *pk, UDPDataIndicationExt *info);

  virtual double getWeightedMean(std::list<double> *l);
  virtual void getVarianceMean(std::list<double> *l, double &mean, double &variance);
  virtual void calculateNeighMeanPhy(double &pow, double &snr, double &per);
  virtual double calculateNeighMeanSnr(void);
  virtual double calculateNeighMeanPow(void);
  virtual double calculateNeighMeanPer(void);

  virtual void calculateAllMeanNeighbourood(struct nodeinfo &info);

  virtual double getDistanceNextHop(void);
  virtual IPv4Address getNextHopAddress(void);

  virtual double getL3Metric(void);

  virtual double calcMeanNeighDistance(void);
  virtual double calcApproachingNeigh(void);
  virtual double calcNextApproaching(void);

  virtual double calcApproachingVal(Coord posA, Coord velA, Coord posB, Coord velB);

  virtual void updateMyInfoVector(struct nodeinfo *info);
  virtual void setVariancesMeans(struct nodeinfo *info);

  virtual void makeStat(struct nodeinfo *myInfo, struct nodeinfo *nextInfo, struct nodeinfo *neighbourInfo);

  virtual void printStreamInfo(std::ostream& os, struct nodeinfo *i);
  virtual void printFileStatHeader(void);

private:
  void updateForce(void);

  void fillMyInfo(struct nodeinfo &info);
  void fillNextInfo(struct nodeinfo &info);
  void fillNeighInfo(struct nodeinfo &info);

public:
  UDPStatisticsApp() {};
  virtual ~UDPStatisticsApp();
};

} /* namespace inet */

#endif /* INET_APPLICATIONS_UDPAPP_UDPSTATISTICSAPP_H_ */
