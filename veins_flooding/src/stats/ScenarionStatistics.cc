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

#include "ScenarionStatistics.h"
#include <iostream>
using std::cerr;
using std::endl;
Define_Module(ScenarionStatistics);


void ScenarionStatistics::initialize(int stage) {

    if (stage == 0) {
        allBeaconsReceivedSignal = registerSignal("allBeaconsReceivedSignal");
        allNewWarningsReceivedSignal = registerSignal("allNewWarningsReceivedSignal");
        allWarningsReceivedSignal = registerSignal("allWarningsReceivedSignal");
        allMessagesReceivedSignal = registerSignal("allMessagesReceivedSignal");
        allBeaconsReceived = allWarningsReceived = newWarningsReceived = allMessagesReceived = 0;
        numAccidentsOccurred = 0;

        //Added
        allDublicateReceivedSignal = registerSignal("allDublicateReceivedSignal");
        allDuplicateReceived = 0;

    }
}

void ScenarionStatistics::updateAllBeaconsReceived() {
    ++allBeaconsReceived;
    emit(allBeaconsReceivedSignal, allBeaconsReceived);
}

void ScenarionStatistics::updateNewWarningsReceived() {
    ++newWarningsReceived;
    emit(allNewWarningsReceivedSignal, newWarningsReceived);
    // cerr << "num warnings: " << newWarningsReceived << simTime().str() << endl;
}

void ScenarionStatistics::updateAllWarningsReceived() {
    emit(allWarningsReceivedSignal, ++allWarningsReceived);
}

void ScenarionStatistics::updateAllMessagesReceived() {
    emit(allMessagesReceivedSignal, ++allMessagesReceived);
}

void ScenarionStatistics::incrementAccidentOccurred() {
    emit(numAccidentsSignal, ++numAccidentsOccurred);
}

// Added
void ScenarionStatistics::updateAllDuplicateReceived() {
    emit(allDublicateReceivedSignal, ++allDuplicateReceived);
}

void ScenarionStatistics::finish() {

}




