using AllegianceInterop;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;

namespace Wopr.Strategies
{
    public class CommanderResearchAndExpand : StrategyBase
    {
        private Queue<String> _investmentQueue = new Queue<string>();

        public CommanderResearchAndExpand() : base(Wopr.Constants.StrategyID.CommanderResearchAndExpand, TimeSpan.MaxValue)
        {
        }

        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            messageReceiver.FMD_S_PAYDAY += MessageReceiver_FMD_S_PAYDAY;
            messageReceiver.FMD_S_SHIP_STATUS += MessageReceiver_FMD_S_SHIP_STATUS;
            messageReceiver.FMD_S_BUCKET_STATUS += MessageReceiver_FMD_S_BUCKET_STATUS;
        }

       
        private void MessageReceiver_FMD_S_BUCKET_STATUS(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BUCKET_STATUS message)
        {
            if (message.sideID != SideIndex)
                return;

            var side = client.GetSide();
            var completedBucket = side.GetBucket(message.iBucket);
            var completedBucketName = completedBucket.GetName();

            if (completedBucketName.StartsWith(".") == true && completedBucketName.Contains("Miner") == true)
            {
                QueueMinerIfRequired();
                
            }
        }

        private void QueueMinerIfRequired()
        {
            if (ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Miner")).Count() < ClientConnection.GetCore().GetMissionParams().GetMaxDrones())
            {
                QueueTech(".Miner");
            }
        }

        private void MessageReceiver_FMD_S_SHIP_STATUS(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
//            Log($@"FMD_S_SHIP_STATUS:
//\tshipID:{message.shipID}
//\thullID:{message.status.GetHullID()}
//\thullName:{client.GetCore().GetHullType(message.status.GetHullID()).GetName()}
//\tshipName:{client.GetCore().GetShip(message.shipID).GetName()}
//\tship.Autopilot: {client.GetCore().GetShip(message.shipID).GetAutopilot()}");

            var ship = client.GetCore().GetShip(message.shipID);

            if (ship.GetName().StartsWith(".") == true)
            {
                if(ship.GetName().Contains("Outpost") == true)
                    PlaceOutpost(ship);
                else if(ship.GetName().Contains("Teleport") == true)
                    PlaceTeleport(ship);
            }
            
        }

        private void PlaceTeleport(IshipIGCWrapper ship)
        {
            Log("\tPlaceTeleport");
        }

        private void PlaceOutpost(IshipIGCWrapper ship)
        {
            Log("\tPlaceOutpost");

            var foundClusters = ClientConnection.GetCore().GetClusters();
            var stations = ClientConnection.GetSide().GetStations();
            

            var homeStation = stations.Where(p => p.GetCluster().GetHomeSector() == true).FirstOrDefault();
            var homeCluster = homeStation.GetCluster();

            var asteroidNames = homeCluster.GetAsteroids().ToDictionary(r => r.GetObjectID(), p => p.GetName());

            Log("\t\tHome asteroids");
            foreach (var name in asteroidNames.Values)
            {
                Log($"\t\t\t{name}");
            }

            var homeRock = homeCluster.GetAsteroids().Where(p => p.GetName().StartsWith("He") == false && p.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(p.GetName()) == false).FirstOrDefault();

            Log($"\t\thomeRock: {homeRock.GetName()}");

            var nextSectorTechRocks = homeCluster.GetWarps()
                .Select(p => p.GetDestination().GetCluster())
                .Select(r => r.GetAsteroids())
                .Select(q => q.Where(s => s.GetName().StartsWith("He") == false && s.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(s.GetName()) == false).FirstOrDefault())
                .Where(t => t != null && String.IsNullOrWhiteSpace(t.GetName()) == false);

            foreach (var nextSectorTechRock in nextSectorTechRocks)
                Log($"\t\tnextSectorTechRock: {nextSectorTechRock.GetName()}");

            var viableNextSectors = nextSectorTechRocks.Where(p => String.IsNullOrWhiteSpace(p.GetName()) == false && String.IsNullOrWhiteSpace(homeRock.GetName()) == false  && p.GetName()[0] == homeRock.GetName()[0]);

            foreach (var viableNextSectorItem in viableNextSectors)
                Log($"\t\tviableNextSector: {viableNextSectorItem.GetName()}");

            var viableNextSector = viableNextSectors.FirstOrDefault();

            var targetNextSector = homeCluster.GetWarps().FirstOrDefault().GetDestination().GetCluster();
            if (viableNextSector != null)
                targetNextSector = viableNextSector.GetCluster();

            Log($"targeting {targetNextSector.GetName()} for possible future tech base, will build outpost forward of this sector.");

            // Find the next adjacent sector that does not lead back to the home sector! (We want a sector that is at least two hops away!)
            //var targetBuildCluster = targetNextSector.GetWarps()
            //    .Where(p => 
            //        p.GetDestination().GetCluster().GetObjectID() != homeCluster.GetObjectID() // Don't go back to the home sector!
            //        && p.GetDestination().GetCluster().GetWarps().Exists(r => r.GetDestination().GetCluster().GetObjectID() == homeCluster.GetObjectID()) == false)
            //     .FirstOrDefault()
            //     ?.GetCluster(); // Don't go to a sector that leads back to the home sector.

            var targetBuildCluster = targetNextSector.GetWarps()
                .OrderByDescending(p => new DijkstraPathFinder(GameInfo.Clusters, p.GetDestination().GetCluster().GetObjectID(), homeCluster.GetObjectID()).GetDistance(p.GetDestination().GetCluster().GetObjectID(), homeCluster.GetObjectID()))
                .Select(p => p.GetDestination().GetCluster())
                .FirstOrDefault();

            // Find the next forward sector if it's been found yet. We are only interested a sector that is two hops from home base.
            //foreach (var nextSectorWarp in targetNextSector.GetWarps())
            //{
            //    var d = new DijkstraPathFinder(ClientConnection.GetCore(), nextSectorWarp.GetDestination().GetCluster(), homeCluster);
            //}

            bool foundTarget = false;

            if (targetBuildCluster != null)
            {
                Log($"\t\ttargeting {targetBuildCluster.GetName()} for outpost cluster.");

                // find closest rock to the center
                VectorWrapper centerPoint = new VectorWrapper(0, 0, 0);
                var targetRock = targetBuildCluster.GetAsteroids()
                    .Where(p => p.GetName().StartsWith("a") == true)
                    .OrderBy(p => (centerPoint - p.GetPosition()).LengthSquared())
                    .FirstOrDefault();

                if (targetRock != null)
                {
                    Log($"\t\ttargeting {targetRock.GetName()} for outpost rock.");

                    ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)CommandID.c_cidBuild, targetRock.GetObjectType(), targetRock.GetObjectID(), targetRock, true);

                    //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetRock, (sbyte)CommandID.c_cidBuild);
                    //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, targetRock, (sbyte)CommandID.c_cidBuild);

                    foundTarget = true;
                    //ship.SetAutopilot(true);
                }
                else
                {
                    Log($"Couldn't find a target rock in cluster: {targetBuildCluster.GetName()}");
                }
            }
            else
            {
                Log($"Couldn't find a target cluster to build in.");
            }

            if (foundTarget == false)
            {
                Log($"Couldn't find a target build rock, moving to sector: {targetNextSector.GetName()} while we wait for the rock to be found.");

                var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 0, 0, 0, targetNextSector.GetObjectID(), true);
                var command = ship.GetDefaultOrder(buoy);

                ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)command, buoy.GetObjectType(), buoy.GetObjectID(), buoy, true);


                //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, buoy, command);

                //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetNextSector, (sbyte)CommandID.c_cidGoto);
                //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, targetNextSector, (sbyte)CommandID.c_cidGoto);
            }
            
        }

        private void MessageReceiver_FMD_S_PAYDAY(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PAYDAY message)
        {
            QueueMinerIfRequired();

            if (_investmentQueue.Count > 0)
            {
                Log($"_investmentQueue.Count: {_investmentQueue.Count}");

                while (_investmentQueue.Count > 0)
                {
                    string techName = _investmentQueue.Peek();

                    if (InvestInTech(techName) == true)
                    {
                        _investmentQueue.Dequeue();
                    }
                    else
                    {
                        break;
                    }
                }

                //for (string techName = _investmentQueue.Peek(); InvestInTech(techName) == true && _investmentQueue.Count > 0; _investmentQueue.Dequeue(), techName = _investmentQueue.Peek()) ;
            }
        }

        public override void Start()
        {
            //ClientConnection.TestMapMaker();

            QueueTech(".Miner");
            QueueTech("Outpost");
            QueueTech("Teleport"); 
        }

        private void QueueTech(string techName)
        {
            if (_investmentQueue.Contains(techName) == false)
            {
                Log($"Queueing tech: {techName}");
                _investmentQueue.Enqueue(techName);
            }
        }

        private bool InvestInTech(string techName)
        {
            var side = ClientConnection.GetSide();
            var buckets = side.GetBuckets();
            var money = ClientConnection.GetMoney();
            //var station = side.GetCu

            foreach (var bucket in buckets)
            {
                if (bucket.GetName().StartsWith(techName) == true && side.CanBuy(bucket) == true)
                {
                    if (money > bucket.GetPrice()/* && bucket.GetPercentComplete() <= 0*/)
                    {
                        Log($"Adding money to bucket {bucket.GetName()}. Bucket wants: {bucket.GetBuyable().GetPrice()}, we have: {money}");
                        ClientConnection.AddMoneyToBucket(bucket, bucket.GetPrice());
                        return true;
                    }
                    else
                    {
                        Log($"Not enough money to buy {techName}, money: {money}, bucketPrice: {bucket.GetPrice()}");
                        //Log("Already building: " + bucket.GetPercentComplete());
                        return false;
                    }
                }
            }

            return false;
        }
    }
}
