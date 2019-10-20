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

#ifndef FLOODING_FLOODING_H_
#define FLOODING_FLOODING_H_

/*
 *
 */

#include "omnetpp.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIColor.h"

#include "stats/ScenarionStatistics.h"

#include <vector>

using Veins::TraCIMobility;
using Veins::TraCICommandInterface;
using Veins::AnnotationManager;
using std::vector;



class flooding : public BaseWaveApplLayer {

    public:

        virtual void initialize(int stage);

        virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj);

    protected:
        TraCIMobility* traci;

        // Added
        TraCICommandInterface* traciCom;
        TraCICommandInterface::Vehicle* traciVehicle;

        ScenarionStatistics* stats;
        vector<WaveShortMessage*> warningMessages;
        simsignal_t beaconReceivedSignal;
        simsignal_t warningReceivedSignal;
        simsignal_t newWarningReceivedSignal;
        simsignal_t messageReceivedSignal;
        simsignal_t nodeLatencySignal;
        simtime_t lastDroveAt;
        bool sentMessage;
        bool color_flag;
        long indexOfAccidentNode;

        bool first_dis;
        simtime_t lastSendTime;

        // Added
        simsignal_t alreadyReceivedSignal;

    protected:
        virtual void onBeacon(WaveShortMessage *wsm);
        virtual void onData(WaveShortMessage *wsm);
        virtual void handlePositionUpdate(cObject *obj);
        virtual void sendMessage(std::string blockedRoadId);
        virtual WaveShortMessage* prepareMessage(std::string name, int dataLengthBits,
                t_channel channel, int priority, int rcvId, int serial=0,
                simtime_t time = simTime(),
                std::string road_ID = "");

        virtual std::list<std::string> NextRoutes();
};

#endif /* FLOODING_FLOODING_H_ */
