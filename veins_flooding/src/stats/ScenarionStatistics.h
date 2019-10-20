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

#ifndef STATS_SCENARIONSTATISTICS_H_
#define STATS_SCENARIONSTATISTICS_H_

#include <omnetpp.h>
using namespace omnetpp;
/*
 *
 */
class ScenarionStatistics: public cSimpleModule {
    public:
        void updateAllBeaconsReceived();
        void updateNewWarningsReceived();
        void updateAllWarningsReceived();
        void updateAllMessagesReceived();
        int getNumberOfAccidentsOccurred() {
            return numAccidentsOccurred;
        }
        void incrementAccidentOccurred();

        // Added
        void updateAllDuplicateReceived();

    protected:
        int allBeaconsReceived;
        int newWarningsReceived;
        int allWarningsReceived;
        int allMessagesReceived;
        int numAccidentsOccurred;


        simsignal_t allBeaconsReceivedSignal;
        simsignal_t allNewWarningsReceivedSignal;
        simsignal_t allWarningsReceivedSignal;
        simsignal_t allMessagesReceivedSignal;
        simsignal_t numAccidentsSignal;

        // Added
        int allDuplicateReceived;
        simsignal_t allDublicateReceivedSignal;

    protected:
        virtual void initialize(int stage);
        virtual void finish();
};

class ScenarionStatisticsAccess {

    public: ScenarionStatisticsAccess() {

    }

    ScenarionStatistics* getIfExists() {
        return dynamic_cast<ScenarionStatistics*>(getSimulation()->getModuleByPath("stats"));
    }
};



#endif /* STATS_SCENARIONSTATISTICS_H_ */
