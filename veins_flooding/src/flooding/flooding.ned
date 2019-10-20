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

package flooding_1.flooding;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple flooding extends BaseWaveApplLayer {
    
	@class(flooding);
	@display("i=block/app2");
	int indexOfAccidentNode = default(0);
	
	@signal[warningReceivedSignal](type=long); 
	@statistic[warningReceivedSignal](record=count,vector; description="Warning Message Received");
	@signal[beaconReceivedSignal](type=long); 
	@statistic[beaconReceivedSignal](record=count,vector; description="Beacon Message Received");
	@signal[messageReceivedSignal](type=long); 
	@statistic[messageReceivedSignal](record=count,vector; description="Message Received");
	@signal[newWarningReceivedSignal](type=long); 
	@statistic[newWarningReceivedSignal](record=count,vector; description="New Warning Message Received");	
	
	// Added
	@signal[alreadyReceivedSignal](type=long); 
	@statistic[alreadyReceivedSignal](record=count,vector; description="Number of Dublicate Received Message");
	@signal[nodeLatencySignal](type=long);
	@statistic[nodeLatencySignal](record=count,vector; description="Latency for Each Node");
	
}