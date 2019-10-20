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

package flooding_1.simulations;

import flooding_1.stats.ScenarionStatistics;
////////////////////////////////////////////////////////////// 
import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;
//////////////////////////////////////////////////////////////
import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManagerLaunchd;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import org.car2x.veins.modules.world.annotations.AnnotationManager;

network scenario
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters) 
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters) 
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=5750.7275,5021.5063");

    submodules:
        obstacles: ObstacleControl {
            parameters:
                @display("p=1106.5076,1519.5837");
        }
        annotations: AnnotationManager {
            parameters:
                @display("p=1115.4551,1109.49");
        }

        connectionManager: ConnectionManager {
            parameters:
                @display("p=1116.9463,1203.4387;i=abstract/multicast");
        }

        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=1112.4725,1310.8087;i=misc/globe");
        }

        manager: TraCIScenarioManagerLaunchd {
            parameters:

                @display("p=1112.4725,1421.1613");
        }

        stats: ScenarionStatistics {
            parameters:
                @display("p=1110.9813,1626.9537");
        }

    connections allowunconnected:
}
