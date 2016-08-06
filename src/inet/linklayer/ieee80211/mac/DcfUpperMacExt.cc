//
// Copyright (C) 2015 Andras Varga
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
// Author: Andras Varga
//

#include <ieee80211/mac/DcfUpperMacExt.h>
#include "Ieee80211Mac.h"
#include "IRx.h"
#include "IContention.h"
#include "ITx.h"
#include "MacUtils.h"
#include "MacParameters.h"
#include "FrameExchanges.h"
#include "DuplicateDetectors.h"
#include "IFragmentation.h"
#include "IRateSelection.h"
#include "IRateControl.h"
#include "IStatistics.h"
#include "inet/common/INETUtils.h"
#include "inet/common/queue/IPassiveQueue.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.h"

#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/linklayer/common/Ieee802CtrlExt.h"

namespace inet {
namespace ieee80211 {

Define_Module(DcfUpperMacExt);

inline double fallback(double a, double b) {return a!=-1 ? a : b;}
inline simtime_t fallback(simtime_t a, simtime_t b) {return a!=-1 ? a : b;}


void DcfUpperMacExt::lowerFrameReceived(Ieee80211Frame *frame)
{
    Enter_Method("lowerFrameReceived(\"%s\")", frame->getName());

    //transmissionQueue.length() >= maxQueueSize
    Ieee80211ReceptionIndication *c = check_and_cast<Ieee80211ReceptionIndication *>(frame->removeControlInfo());

    Ieee802CtrlExt *ctrl = new Ieee802CtrlExt();
    ctrl->setRcvPow(c->getRecPow());
    ctrl->setRcvSnr(c->getSnr());
    ctrl->setRcvPER(c->getPacketErrorRate());
    ctrl->setQueueMacAbs(transmissionQueue.getLength());
    ctrl->setQueueMacPerc(((double) transmissionQueue.getLength()) / ((double) maxQueueSize));

    //delete frame->removeControlInfo();          //TODO
    /*Ieee80211ReceptionIndication *c = check_and_cast<Ieee80211ReceptionIndication *>(frame->removeControlInfo());

    EV_DEBUG << "detailedInfo Info: " << c->detailedInfo() << std::endl;
    EV_DEBUG << "getClassName Info: " << c->getClassName() << std::endl;
    EV_DEBUG << "getDescriptor Info: " << c->getDescriptor() << std::endl;
    EV_DEBUG << "getFullName Info: " << c->getFullName() << std::endl;
    EV_DEBUG << "getFullPath Info: " << c->getFullPath() << std::endl;
    EV_DEBUG << "getName Info: " << c->getName() << std::endl;
    EV_DEBUG << "getOwner Info: " << c->getOwner() << std::endl;
    EV_DEBUG << "info Info: " << c->info() << std::endl;

    EV_DEBUG << "getAirtimeMetric Info: " << c->getAirtimeMetric() << std::endl;
    EV_DEBUG << "getBitErrorCount Info: " << c->getBitErrorCount() << std::endl;
    EV_DEBUG << "getBitErrorRate Info: " << c->getBitErrorRate()<< std::endl;
    EV_DEBUG << "getChannel Info: " << c->getChannel() << std::endl;
    EV_DEBUG << "getLossRate Info: " << c->getLossRate() << std::endl;
    EV_DEBUG << "getMode Info: " << c->getMode() << std::endl;
    EV_DEBUG << "getMinRSSI Info: " << c->getMinRSSI() << std::endl;
    EV_DEBUG << "getMinSNIR Info: " << c->getMinSNIR() << std::endl;
    EV_DEBUG << "getPacketErrorRate Info: " << c->getPacketErrorRate() << std::endl;
    EV_DEBUG << "getRecPow Info: " << c->getRecPow() << std::endl;
    EV_DEBUG << "getSnr Info: " << c->getSnr() << std::endl;
    EV_DEBUG << "getSymbolErrorCount Info: " << c->getSymbolErrorCount() << std::endl;
    EV_DEBUG << "getSymbolErrorRate Info: " << c->getSymbolErrorRate() << std::endl;
    EV_DEBUG << "getTestFrameDuration Info: " << c->getTestFrameDuration() << std::endl;
    EV_DEBUG << "getTestFrameError Info: " << c->getTestFrameError() << std::endl;
    EV_DEBUG << "getTestFrameSize Info: " << c->getTestFrameSize() << std::endl;*/

    take(frame);

    if (!utils->isForUs(frame)) {
        EV_INFO << "This frame is not for us" << std::endl;
        delete frame;
        if (frameExchange)
            frameExchange->corruptedOrNotForUsFrameReceived();
    }
    else {
        // offer frame to ongoing frame exchange
        IFrameExchange::FrameProcessingResult result = frameExchange ? frameExchange->lowerFrameReceived(frame) : IFrameExchange::IGNORED;
        bool processed = (result != IFrameExchange::IGNORED);
        if (processed) {
            // already processed, nothing more to do
            if (result == IFrameExchange::PROCESSED_DISCARD)
                delete frame;
        }
        else if (Ieee80211RTSFrame *rtsFrame = dynamic_cast<Ieee80211RTSFrame *>(frame)) {
            sendCts(rtsFrame);
            delete rtsFrame;
        }
        else if (Ieee80211DataOrMgmtFrame *dataOrMgmtFrame = dynamic_cast<Ieee80211DataOrMgmtFrame *>(frame)) {
            if (!utils->isBroadcastOrMulticast(frame))
                sendAck(dataOrMgmtFrame);
            if (duplicateDetection->isDuplicate(dataOrMgmtFrame)) {
                EV_INFO << "Duplicate frame " << frame->getName() << ", dropping\n";
                delete dataOrMgmtFrame;
            }
            else {

                if (!utils->isFragment(dataOrMgmtFrame)){
                    dataOrMgmtFrame->setControlInfo(ctrl);      // ADDED by Angelo Trotta
                    mac->sendUp(dataOrMgmtFrame);
                }
                else {
                    Ieee80211DataOrMgmtFrame *completeFrame = reassembly->addFragment(dataOrMgmtFrame);
                    if (completeFrame){
                        completeFrame->setControlInfo(ctrl);      // ADDED by Angelo Trotta
                        mac->sendUp(completeFrame);
                    }
                }
            }
        }
        else {
            EV_INFO << "Unexpected frame " << frame->getName() << ", dropping\n";
            delete frame;
        }
    }
}

} // namespace ieee80211
} // namespace inet

