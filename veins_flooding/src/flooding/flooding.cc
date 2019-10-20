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

#include "flooding.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include <iostream>
#include <list>

using Veins::TraCIMobility;
using Veins::TraCIMobilityAccess;

Define_Module(flooding);

void flooding::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);

    if (stage == 0) {

        traci = TraCIMobilityAccess().get(getParentModule());
        traciCom = traci->getCommandInterface();
        traciVehicle = traci->getVehicleCommandInterface();

        stats = ScenarionStatisticsAccess().getIfExists();
        ASSERT(stats);

        beaconReceivedSignal = registerSignal("beaconReceivedSignal");
        warningReceivedSignal = registerSignal("warningReceivedSignal");
        messageReceivedSignal = registerSignal("messageReceivedSignal");
        newWarningReceivedSignal = registerSignal("newWarningReceivedSignal");
        nodeLatencySignal = registerSignal("nodeLatencySignal");

        indexOfAccidentNode = par("indexOfAccidentNode").intValue();

        // Added
        alreadyReceivedSignal = registerSignal("alreadyReceivedSignal");
        // This is useless
        lastDroveAt = simTime();
        // This flag allows to sent messages.
        sentMessage = false;
        // This flag is for first dissemination.
        first_dis = true;
        // Last time a messages has been send.
        lastSendTime = simTime();
        // Color Flag to change the color of the traffic lights.
        color_flag = true;
    }
}

void flooding::receiveSignal(cComponent *source, simsignal_t signalID, cComponent::cObject *obj) {

    //std::cerr << "This is called !!!" << endl;

    //BaseWaveApplLayer::receiveSignal(source, signalID, obj, details);

    // MACRO to denote module class member function as callable from other modules.
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
}

void flooding::onBeacon(WaveShortMessage *wsm) {
    // not used for this algorithm
}

// This function is called each time data have been received.
// Fixed the re-transmittion problem.
void flooding::onData(WaveShortMessage *wsm) {

    std::cerr << "This is called by Car Type " << traciVehicle->getTypeId() << endl;

    // statistics recording
    emit(warningReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
    stats->updateAllWarningsReceived();
    stats->updateAllMessagesReceived();

    // Check if message is received earlier.
    bool messageIsRepeat = false;

    size_t i;
    for ( i = 0; i < warningMessages.size(); ++i) {
        WaveShortMessage* warningMessage = warningMessages[i];
        if (wsm->getTreeId() == warningMessage->getTreeId()) {

            std::cerr << "I have received this already!" << endl;
            messageIsRepeat = true;

            //Count the repeated messages.
            emit(alreadyReceivedSignal, 1);
            stats->updateAllDuplicateReceived();
        }
    }

    // Rebroadcast if we have a new message and you are not EV.
    if (!messageIsRepeat) {

        // This means we have a new message.
        // Calculate and Store Latency (in ms).
        double node_latency = (simTime().dbl()*1000) - (wsm->getEmergency_data().lastSeenAt.dbl()*1000);
        emit(nodeLatencySignal, node_latency);

        // If its a new message change the color of car.
        if (traciVehicle->getTypeId() != "EV") {
            if (color_flag) {
                traciVehicle->setColor(Veins::TraCIColor::fromTkColor("AntiqueWhite"));
                color_flag = false;
            }
            else {
                traciVehicle->setColor(Veins::TraCIColor::fromTkColor("CornflowerBlue"));
                color_flag = true;
            }
        }

        // Re-transmit.
        sendWSM(wsm->dup());
        stats->updateNewWarningsReceived();
        emit(newWarningReceivedSignal, 1);
        warningMessages.push_back(wsm->dup());
    }
}



void flooding::handlePositionUpdate(cComponent::cObject *obj) {


    // Here we have to decide what to do with the Position Update.
    // Might need to check if it's a EV or not and then do some basic actions.
    // If it's in the same road for example then stop moving.
    // The data transmitted must have this kind of info inside.
    // So first we need to fix the data.

    // Initialize Messages only in the case of the EV
    if (traciVehicle->getTypeId() == "EV") {

        sendMessage(traci->getRoadId());
    }

    else {

        //TODO: Here we must specify how the other cars should behave.
        traciVehicle->slowDown(30, 3);
    }
}


// This is called when we send new data.
// Right now it is sending data when a new car enters
// and does not sent any new data of this car after that.
// Need to fix here so only EV can broadcast
// and continue to broadcast while its going to the destination.
// After that we need to fix what "DATA-MESSAGE" it is broadcasting.
void flooding::sendMessage(std::string blockedRoadId) {


    // Only when we have EV.
    if (traciVehicle->getTypeId() == "EV") {

        // TODO:Fix the condition of sending messages.
        double timeout = simTime().dbl() - lastSendTime.dbl();

        // For DEBUG
        //std::cerr << "This is called again in : " << simTime().dbl() << endl;
        //std::cerr << "The last message was send in : " << lastSendTime.dbl() << endl;
        //std::cerr << "Difference : " << lastSendTime.dbl() << " \n" << endl;


        // If its the first time.
        if(first_dis) {

            // DEBUG
            std::cerr << "\n Sending first message! \n" << endl;
            // Set the flag to false
            first_dis = false;
            // Store latest dissemination  time.
            lastSendTime = simTime();
            // Send the message.
            t_channel channel = dataOnSch ? type_SCH : type_CCH;
            WaveShortMessage* wsm = prepareMessage("data", dataLengthBits, channel, dataPriority, -1,2,
                    lastSendTime, traciVehicle->getRoadId());
            wsm->setWsmData(blockedRoadId.c_str());
            sendWSM(wsm);

        }

        // If 2 seconds have passed from last sent.
        else {

            if (timeout > 2.0) {

                // DEBUG
                std::cerr << "\n Time to send a new message! \n" << endl;
                std::cerr << "The last message was send in : " << lastSendTime.dbl() << "\n" << endl;
                //std::cerr << "The current road of EV is: " << traciVehicle->getRoadId() << "\n" << endl;

                // Store latest dissemination  time.
                lastSendTime = simTime();
                // Send the message.
                t_channel channel = dataOnSch ? type_SCH : type_CCH;
                WaveShortMessage* wsm = prepareMessage("data", dataLengthBits, channel, dataPriority, -1,2,
                        lastSendTime, traciVehicle->getRoadId());
                wsm->setWsmData(blockedRoadId.c_str());
                // TODO: Add stats of how many messages EV transmits.
                sendWSM(wsm);
            }

        }
    }
}



// Replacement of prepareWSM so we can sent are own data.
WaveShortMessage*  flooding::prepareMessage(std::string name, int lengthBits, t_channel channel,
        int priority, int rcvId, int serial,
        simtime_t time, std::string road_ID) {


    // What data we have from BaseWaveApplLayer
    //
    //   Defined in initialazation inside BaseWaveApplLayer.cc
    //
    // myId = getParentModule()->getIndex();
    // headerLength = par("headerLength").intValue();
    //
    //
    //   Defined in header file BaseWaveApplLayer.h
    //
    //Coord curPosition;
    //
    // and inside BaseWaveApplLayer::handlePositionUpdate we get
    //
    //  curPosition = mobility->getCurrentPosition();


    WaveShortMessage* wsm = new WaveShortMessage(name.c_str());
    wsm->addBitLength(headerLength);
    wsm->addBitLength(lengthBits);

    switch (channel) {
        case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
        case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
    }

    wsm->setPsid(0);
    wsm->setPriority(priority);
    wsm->setWsmVersion(1);
    wsm->setTimestamp(simTime());
    wsm->setSenderAddress(myId);
    wsm->setRecipientAddress(rcvId);
    wsm->setSenderPos(curPosition);
    wsm->setSerial(serial);

    if (name == "beacon") {
        DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }
    if (name == "data") {
        DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }

    // Add my data
    emergency_msg * msg = new emergency_msg();
    msg->lastSeenAt = time;
    msg->current_road.assign(road_ID);
    msg->unique_id = 0;
    msg->vType = "EV";
    msg->evPos = traci->getCurrentPosition();
    msg->evSpeed = traci->getCurrentSpeed().length();
    msg->evAngle = traci->getAngleRad();

    // Calculate next 3 roads ID and store it in message.
    std::list<std::string> next_roads = NextRoutes();
    msg->future_roads.assign(next_roads.begin(),next_roads.end());

    wsm->setEmergency_data(*msg);

    // Print the packet data
    std::cerr << "The new data packet is: \n"   << msg->lastSeenAt.dbl() << "\n"
                                                << msg->current_road << "\n"
                                                << msg->unique_id << "\n"
                                                << msg->vType << "\n"
                                                << msg->evPos << "\n"
                                                << msg->evSpeed << "\n"
                                                << msg->evAngle << endl;
    for (auto const& i: msg->future_roads) { std::cerr << i << endl;}
    std::cerr << endl;


    return wsm;
}



// Calculates the next 3 routes of EV
std::list<std::string> flooding::NextRoutes(){

    // Current Route
    std::string  cur_road = traciVehicle->getRoadId();
    std::cerr << "Current Road is : " << cur_road  <<"\n " << endl;
    // List of Routes the EV will take.
    std::list<std::string> list = traciVehicle->getPlannedRoadIds();

    std::string one_r;
    std::string two_r;
    std::string three_r;

    int inner_flag = 0;
    for (auto const& i: list) {

        if (inner_flag ==3) {
            three_r.assign(i);
            inner_flag = 3;
            std::cerr << i << "\n";
        }

        if (inner_flag ==2) {
            two_r.assign(i);
            inner_flag = 3;
            std::cerr << i << "\n";
        }

        if (inner_flag ==1) {
            one_r.assign(i);
            inner_flag = 2;
            std::cerr << i << "\n";
        }

        if (i.compare(cur_road) == 0) {
            std::cerr << "Current : " << i << "\n";
            inner_flag = 1;
        }
    }

    // Future Routes
    std::list<std::string> future_roads = {one_r,two_r,three_r};
    std::cerr << "\n Next 3 roads are \n";
    for (auto const& i: future_roads) { std::cerr << i << "\n";}

    return future_roads;
}
