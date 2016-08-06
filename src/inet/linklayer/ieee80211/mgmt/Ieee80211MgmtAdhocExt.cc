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

#include "inet/common/INETUtils.h"

#include "inet/linklayer/common/Ieee802CtrlExt.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocExt.h"

#include <Ieee80211ControlInfo_m.h>

namespace inet {

namespace ieee80211 {

Define_Module(Ieee80211MgmtAdhocExt);


cPacket *Ieee80211MgmtAdhocExt::decapsulate(Ieee80211DataFrame *frame)
{
    double snr, pow, per, qAbs, qPerc;
    snr = pow = per = qAbs = qPerc = 0;

    cObject *c = frame->getControlInfo();
    if (c) {
        /*EV_DEBUG << "YES CONTROL in Ieee80211MgmtAdhocExt::decapsulate" << std::endl;
        EV_DEBUG << "detailedInfo Info: " << c->detailedInfo() << std::endl;
        EV_DEBUG << "getClassName Info: " << c->getClassName() << std::endl;
        EV_DEBUG << "getDescriptor Info: " << c->getDescriptor() << std::endl;
        EV_DEBUG << "getFullName Info: " << c->getFullName() << std::endl;
        EV_DEBUG << "getFullPath Info: " << c->getFullPath() << std::endl;
        EV_DEBUG << "getName Info: " << c->getName() << std::endl;
        EV_DEBUG << "getOwner Info: " << c->getOwner() << std::endl;
        EV_DEBUG << "info Info: " << c->info() << std::endl;*/

        //inet::physicallayer::Ieee80211ReceptionIndication *cExt = check_and_cast<inet::physicallayer::Ieee80211ReceptionIndication *>(c);
        Ieee802CtrlExt *cExt = check_and_cast<Ieee802CtrlExt *>(c);

        pow =  cExt->getRcvPow();
        snr = cExt->getRcvSnr();
        per = cExt->getRcvPER();
        qAbs = cExt->getQueueMacAbs();
        qPerc = cExt->getQueueMacPerc();

        EV_DEBUG << "Ieee80211MgmtAdhocExt::decapsulate Getting POW: " << pow << " and SNR: " << snr << std::endl;

        /*EV_DEBUG << "Ieee80211ReceptionIndication POW: " << pow << std::endl;
        EV_DEBUG << "Ieee80211ReceptionIndication SNR: " << snr << std::endl;*/
    }
    else {
        EV_DEBUG << "NO CONTROL in Ieee80211MgmtAdhocExt::decapsulate" << std::endl;
    }

    cPacket *payload = frame->decapsulate();

    Ieee802CtrlExt *ctrl = new Ieee802CtrlExt();
    ctrl->setSrc(frame->getTransmitterAddress());
    ctrl->setDest(frame->getReceiverAddress());
    ctrl->setRcvPow(pow);
    ctrl->setRcvSnr(snr);
    ctrl->setRcvPER(per);
    ctrl->setQueueMacAbs(qAbs);
    ctrl->setQueueMacPerc(qPerc);

    EV_DEBUG << "Ieee80211MgmtAdhocExt::decapsulate Setting POW: " << pow << " and SNR: " << snr << std::endl;

    int tid = frame->getTid();
    if (tid < 8)
        ctrl->setUserPriority(tid); // TID values 0..7 are UP
    Ieee80211DataFrameWithSNAP *frameWithSNAP = dynamic_cast<Ieee80211DataFrameWithSNAP *>(frame);
    if (frameWithSNAP)
        ctrl->setEtherType(frameWithSNAP->getEtherType());
    payload->setControlInfo(ctrl);

    delete frame;
    return payload;
}


} // namespace ieee80211

} // namespace inet

