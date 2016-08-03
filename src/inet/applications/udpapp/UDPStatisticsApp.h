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

#include "inet/applications/udpapp/UDPBasicBurst.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"

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

        friend std::ostream& operator << (std::ostream& os, neighStruct& n) {
            //os << "Addr: " << n.addr << " Degree: " << n.degree;// << " NeighList: " << neigh;

            os <<
                    //"Addr: " << n.nodeInf.localAddr <<
                    "[" << n.nodeInf.appAddr << "]" <<
                    " Deg: " << n.nodeInf.nodeDegree << " [" << n.nodeInf.meanNodeDegreeNeighbourood << "]" <<
                    " SNR: " << n.nodeInf.snrNeighbourood << " [" << n.nodeInf.meanSnrNeighbourood << "]" <<
                    " Pow: " << n.nodeInf.powNeighbourood << " [" << n.nodeInf.meanPowNeighbourood << "]" <<
                    " Pos: " << n.nodeInf.pos <<
                    " Vel: " << n.nodeInf.velocity << " [" << n.nodeInf.meanVelocityNeighbourood << "]" <<
                    " Dist: " << n.nodeInf.distance << " [" << n.nodeInf.meanDistanceNeighbourood << "]" <<
                    " NHDist: " << n.nodeInf.nextHopDistance << " [" << n.nodeInf.meanNextHopDistanceNeighbourood << "]" <<
                    " Appr: " << n.nodeInf.approaching << " [" << n.nodeInf.meanApproachingNeighbourood << "]" <<
                    " NHAppr: " << n.nodeInf.nextHopApproaching << " [" << n.nodeInf.meanNextHopApproachingNeighbourood << "]" <<
                    "";

            return os;
        }
    } neigh_t;

protected:
    cMessage *autoMsg = nullptr;

private:
  L3Address myAddr;
  int myAppAddr;
  std::map<L3Address, neigh_t> neighbourood;

  //parameters
  int pysicalDataHistorySize;

  IMobility *mob;

protected:
  virtual int numInitStages() const override { return NUM_INIT_STAGES; }
  virtual void initialize(int stage) override;
  virtual void handleMessage(cMessage *msg) override;
  virtual void finish() override;

  virtual void handle100msAutoMsg(void);
  virtual void updateNeighList(void);

  virtual void processStart();

  virtual void processPacket(cPacket *msg);
  virtual cPacket *createPacket();

  virtual void manageReceivedPkt(cPacket *pk, UDPDataIndicationExt *info);

  virtual double getMean(std::list<double> *l);
  virtual void calculateNeighMeanPhy(double &pow, double &snr);
  virtual double calculateNeighMeanSnr(void);
  virtual double calculateNeighMeanPow(void);

  virtual void calculateAllMeanNeighbourood(struct nodeinfo &info);

  virtual double getDistanceNextHop(void);
  virtual IPv4Address getNextHopAddress(void);

  virtual double calcMeanNeighDistance(void);
  virtual double calcApproachingNeigh(void);
  virtual double calcNextApproaching(void);

  virtual double calcApproachingVal(Coord posA, Coord velA, Coord posB, Coord velB);

private:
  void updateForce(void);

  void fillMyInfo(struct nodeinfo &info);
  void fillNextInfo(struct nodeinfo &info);

public:
  UDPStatisticsApp() {};
  virtual ~UDPStatisticsApp();
};

} /* namespace inet */

#endif /* INET_APPLICATIONS_UDPAPP_UDPSTATISTICSAPP_H_ */
