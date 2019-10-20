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

package flooding_1.stats;

simple ScenarionStatistics {
    parameters: 
        @class(ScenarionStatistics); 
        @display("i=block/table2_vl.png");
        @signal[allBeaconsReceivedSignal](type=long);
        @statistic[allBeaconsReceivedSignal](record=count,vector; description="All Beacons Received");
        @signal[allNewWarningsReceivedSignal](type=long);
        @statistic[allNewWarningsReceivedSignal](record=count,vector; description="All New Warnings Received");
        @signal[allWarningsReceivedSignal](type=long);
        @statistic[allWarningsReceivedSignal](record=count,vector; description="All Warnings Received");
        @signal[allMessagesReceivedSignal](type=long);
        @statistic[allMessagesReceivedSignal](record=count,vector; description="All Messages Received");
        //@signal[numAccidentsSignal](type=long);
        //@statistic[numAccidentsSignal](record=count,vector; description="Number Of Occurred Accidents");
        
        // Added
        @signal[allDublicateReceivedSignal](type=long);
        @statistic[allDublicateReceivedSignal](record=count,vector; description="Number of Dublicate Received Message");
}