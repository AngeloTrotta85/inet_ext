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

#include "inet/applications/base/ApplicationPacketBurst_m.h"

namespace inet {

/**
 * UDP application. See NED for more info.
 */
class INET_API UDPBasicBurstExt : public UDPBasicBurst
{
public:
    typedef struct transmissionStatus{
        simtime_t simtimeSent;
        simtime_t simtimeReceivedAck;
        int pktSizeByte;
        int pktCode;
        bool acknowledged;

        friend std::ostream& operator << (std::ostream& os, transmissionStatus& n) {
            //os << "Addr: " << n.addr << " Degree: " << n.degree;// << " NeighList: " << neigh;

            os <<
                    "[" << n.pktCode << "]" <<
                    " Acknowledged?: " << (n.acknowledged ? "OK" : "NO") <<
                    " Sent at: " << n.simtimeSent <<
                    " Received at: " << n.simtimeReceivedAck <<
                    " Pkt Size: " << n.pktSizeByte <<
                    "";

            return os;
        }
    } transmissionStatus_t;

  protected:

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void initialize(int stage) override;

    // chooses random destination address
    virtual L3Address chooseDestAddr();
    virtual cPacket *createPacket();
    virtual cPacket *createAckPacket(ApplicationPacketBurst *msg2ack, int index);
    virtual void handleIncomingPacket(ApplicationPacketBurst *msg);
    virtual void handleIncomingAck(ApplicationPacketBurst *msg);

    virtual void addToMapSec(transmissionStatus_t *stat);
    virtual void addToMapNum(transmissionStatus_t *stat);


    virtual double getMean(std::list<double> *l);
    virtual double getVar(std::list<double> *l);
    void getMeanVar_ThrougputDelayPDR(std::list<transmissionStatus_t> *list, double &throughput_val, double &throughput_var, double &delay_val, double &delay_var, double &pdr_val);

  public:
    UDPBasicBurstExt() {};
    ~UDPBasicBurstExt() {};

    virtual L3Address getDestAddr(void);

    virtual double getThroughputMeanSec(void);
    virtual double getThroughputVarSec(void);
    virtual double getThroughputMeanNum(void);
    virtual double getThroughputVarNum(void);

    virtual double getDelayMeanSec(void);
    virtual double getDelayVarSec(void);
    virtual double getDelayMeanNum(void);
    virtual double getDelayVarNum(void);

    virtual double getPDRSec(void);
    virtual double getPDRNum(void);

    virtual double getMessageLength(void);
    virtual double getMessageLengthSigma(void);
    virtual double getSendInterval(void);
    virtual double getBurstVolume(void);

  private:
  //  int destAddrIdx;
    L3Address myAddr;
    L3Address actualDestAddr;

    std::list<transmissionStatus_t> transmissionsStatusSec;
    std::list<transmissionStatus_t> transmissionsStatusNum;

    //parameters
    int maxStatSizeNum;
    double maxStatSizeSec;
};

} // namespace inet

#endif // ifndef __INET_UDPBASICBURSTEXT_H

