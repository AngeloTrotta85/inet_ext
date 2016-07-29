//
// Copyright (C) 2013 OpenSim Ltd.
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

#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiverExt.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmissionBase.h"

#include "inet/physicallayer/analogmodel/packetlevel/ScalarReception.h"

namespace inet {

namespace physicallayer {

Define_Module(Ieee80211ScalarReceiverExt);

const ReceptionIndication *Ieee80211ScalarReceiverExt::computeReceptionIndication(const ISNIR *snir) const
{
    Ieee80211ReceptionIndication *indication = check_and_cast<Ieee80211ReceptionIndication *>(const_cast<ReceptionIndication *>(FlatReceiverBase::computeReceptionIndication(snir)));

    const Ieee80211TransmissionBase *transmission = check_and_cast<const Ieee80211TransmissionBase *>(snir->getReception()->getTransmission());

    const IReception *reception = snir->getReception();
    //const ScalarReception *scalarreception = (ScalarReception *) reception;
    //const INoise *noise = snir->getNoise();

    const FlatReceptionBase *flatReception = check_and_cast<const FlatReceptionBase *>(reception);
    W minReceptionPower = flatReception->computeMinPower(reception->getStartTime(), reception->getEndTime());

    /*EV << "LALALALA" << std::endl;
    EV << "reception " << reception << std::endl;
    EV << "pow " << minReceptionPower << std::endl;

    EV << "indication " << indication->detailedInfo() << std::endl;
    EV << "indication getMinRSSI " << indication->getMinRSSI() << std::endl;
    EV << "indication getMinSNIR " << indication->getMinSNIR() << std::endl;
    EV << "indication getRecPow " << indication->getRecPow() << std::endl;
    EV << "indication getSnr " << indication->getSnr() << std::endl;
    EV << "transmission " << transmission << std::endl;
    const INoise *noise = snir->getNoise();
    EV << "noise " << noise << std::endl;
    EV << "snir " << snir << std::endl;*/

    //FIXME fill indication
    indication->setMode(transmission->getMode());
    indication->setChannel(const_cast<Ieee80211Channel *>(transmission->getChannel()));

    //W w = ScalarAnalogModelBase::computeReceptionPower(snir->getReception()->getReceiver(), transmission, snir->getReception()->getAnalogModel());

    //const IAnalogModel * analogModel = snir->getReception()->getAnalogModel();
    //analogModel->

    indication->setSnr(indication->getMinSNIR());
    //indication->setLossRate();
    indication->setRecPow(minReceptionPower.get());
    //indication->setAirtimeMetric();
    //indication->setTestFrameDuration();
    //indication->setTestFrameError());
    //indication->setTestFrameSize();

    return indication;
}


} // namespace physicallayer

} // namespace inet

