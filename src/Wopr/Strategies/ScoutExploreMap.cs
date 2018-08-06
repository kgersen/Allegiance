using AllegianceInterop;
using Strategies.Loadouts;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;

namespace Strategies
{
    public class ScoutExploreMap : Wopr.StrategyBase
    {
        private const int SweepHopCount = 16; // The number of waypoint hops a scout will make when doing an orbit of a sector scanning for stuff. 

        private int _currentSectorID = -1;
        private bool _isSweeping = false;
        private IclusterIGCWrapper _navigatingToCluster = null;

        //private bool _sweepLeft = false;
        //private bool _sweepRight = false;
        //private double _currentAngleInRadians;
        //private double _startingAngleInRadians;
        //private double _totalRadiansTraversed;
        //private VectorWrapper _startingPoint;
        //private int _currentSweepHop = 0;

        public ScoutExploreMap() : base(Wopr.Constants.StrategyID.ScoutExploreMap, TimeSpan.MaxValue)
        {
        }

        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            Log("AttachMessages");

            messageReceiver.FMD_S_SET_CLUSTER += MessageReceiver_FMD_S_SET_CLUSTER;
            messageReceiver.FMD_S_SINGLE_SHIP_UPDATE += MessageReceiver_FMD_S_SINGLE_SHIP_UPDATE;
            messageReceiver.FMD_S_SHIP_STATUS += MessageReceiver_FMD_S_SHIP_STATUS;
        }

        private void MessageReceiver_FMD_S_SHIP_STATUS(ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            Log("MessageReceiver_FMD_S_SHIP_STATUS");

            var ship = _client.GetShip();
            string shipName = ship.GetName();

            UpdateUnexploredWarpsList();

            //if (_sweepLeft == true || _sweepRight == true)
            //    NavigateToNextSweepPoint();

            if (ship.GetCluster() == null || message.status.GetSectorID() == _currentSectorID || _isSweeping == true)
            {
                Log($"Skipping message: ship.GetCluster() = {ship.GetCluster()}, message.status.GetSectorID() = {message.status.GetSectorID()}, _currentSectorID = {_currentSectorID}, _isSweeping = {_isSweeping}");
                return;
            }

            if (_navigatingToCluster != null)
            {
                if (_navigatingToCluster.GetObjectID() != ship.GetCluster().GetObjectID())
                {
                    if (GameInfo.UnexploredClustersByObjectID.ContainsKey(_navigatingToCluster.GetObjectID()) == true)
                    {
                        Log($"Entered cluster: {ship.GetCluster().GetName()}, continuing on route to {_navigatingToCluster.GetName()}");
                        return;
                    }
                    else
                    {
                        Log($"Arrived at cluster: {ship.GetCluster().GetName()}, but destination cluster {_navigatingToCluster.GetName()} has already been swept. Cancelling navigation, checking for next step.");
                        _navigatingToCluster = null;
                    }
                }
                else
                {
                    Log($"Arrived at cluster: {ship.GetCluster().GetName()}, checking for next step.");
                    _navigatingToCluster = null;
                }
            }


            _currentSectorID = message.status.GetSectorID();
            //_sweepLeft = false;
            //_sweepRight = false;
            //_startingAngleInRadians = 0;
            //_currentAngleInRadians = 0;
            //_totalRadiansTraversed = 0;
            //_startingPoint = ship.GetPosition();
            //_currentSweepHop = 0;

            var otherFriendlyScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true && p.GetSide().GetObjectID() == ship.GetSide().GetObjectID());
            var otherSweepingScoutShips = otherFriendlyScoutShips.Where(p => p.GetCommandTarget((sbyte)CommandType.c_cmdCurrent)?.GetObjectType() == (short)ObjectType.OT_buoy);

            Log($"otherFriendlyScoutShips: {otherFriendlyScoutShips.Count()}, otherSweepingScoutShips: {otherSweepingScoutShips.Count()}, is unexplored: {GameInfo.UnexploredClustersByObjectID.ContainsKey(ship.GetCluster().GetObjectID())} ");

            // Ship has entered a cluster, find the next target and start scouting.

            // If cluster is unexplored, and less than 2 ships sweeping, start sweeping
            if (GameInfo.UnexploredClustersByObjectID.ContainsKey(ship.GetCluster().GetObjectID()) == true && otherSweepingScoutShips.Count() < 2)
                SweepCluster();

            // If there are two ships sweeping, fly to a point 1/3 of the angle we entered in at, and wait for alephs to appear.
            else if (otherSweepingScoutShips.Count() >= 2)
                FlyToStationPointAndWaitForAelphs();

            // If there are no ships sweeping, go to the next system. 
            else
                SelectNextAelphTarget();




            //var mission = ship.GetMission();

            //var warps = mission.GetWarps();

            //// Get all the clusters that are available in the mission.
            //var missionClusters = mission.GetClusters();

            //var currentShipCluster = ship.GetCluster();
            //var currentMissionCluster = missionClusters.FirstOrDefault(p => p.GetObjectID() == currentShipCluster.GetObjectID());

            //var shipVisibleWarps = currentShipCluster.GetWarps();
            //var allWarpsInCluster = currentMissionCluster.GetWarps();







        }

        private void UpdateUnexploredWarpsList()
        {
            Log("UpdateUnexploredWarpsList()");

            //var ship = _client.GetShip();
            var unexploredWarps = _client.GetCore().GetWarps().Where(p => p.GetDestination().GetCluster().GetAsteroids().Count == 0 && GameInfo.UnexploredClustersByObjectID.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == false);

            foreach (var unexploredWarp in unexploredWarps)
            {
                Log($"Found unexplored cluster: {unexploredWarp.GetDestination().GetCluster().GetName()}, adding cluster's id to the GameInfo.UnexploredClustersByObjectID list.");

                lock (GameInfo.UnexploredClustersByObjectID)
                {
                    if(GameInfo.UnexploredClustersByObjectID.ContainsKey(unexploredWarp.GetDestination().GetCluster().GetObjectID()) == false)
                        GameInfo.UnexploredClustersByObjectID.Add(unexploredWarp.GetDestination().GetCluster().GetObjectID(), unexploredWarp.GetDestination().GetCluster().GetName());
                }
            }

            StringBuilder s = new StringBuilder();
            foreach (var unexploredClusterName in GameInfo.UnexploredClustersByObjectID.Values)
                s.Append(unexploredClusterName + " ");

            Log("Current unexplored clusters: " + s.ToString());
        }

        private void FlyToStationPointAndWaitForAelphs()
        {
            Log("FlyToStationPointAndWaitForAelphs");

            var ship = _client.GetShip();
            var centerPoint = new VectorWrapper(0, 0, 0);
            var shipPosition = ship.GetPosition();

            var startingPoint = shipPosition;
            var startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);

            // 1/3 of 360 degrees in radians.
            double oneThirdAngle = Math.PI * 2 / 3;

            // Move to a new point 1/3 or 2/3 of the total angle and the same distance as the entry aleph.
            double newAngle = startingAngleInRadians + (oneThirdAngle * _random.Next(1, 3));

            VectorWrapper nextPoint = new VectorWrapper((startingPoint - centerPoint).Length() * (float)Math.Cos(newAngle), (startingPoint - centerPoint).Length() * (float)Math.Sin(newAngle), 0);

            var buoy = _client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
            var command = ship.GetDefaultOrder(buoy);
            ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
            ship.SetAutopilot(true);

            List<int> consideredWarpObjectIDs = new List<int>();

            // Wait for Aelphs to appear. 
            Task.Run(() =>
            {
                try
                {
                    _runningTasks++;

                    bool foundNewWarp = false;
                    bool updatedUnexploredClusterListAfterMove = false;
                    for (int i = 0; i < 120 * 100 && _cancellationTokenSource.IsCancellationRequested == false; i++)
                    {
                        var otherScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true);
                        var otherSweepingScoutShips = otherScoutShips.Where(p => p.GetCommandTarget((sbyte)CommandType.c_cmdCurrent)?.GetObjectType() == (short)ObjectType.OT_buoy);

                        if (i % 500 == 0)
                        {
                            Log($"Waiting for Alephs to be found: {i}.");

                            // Once we get within 400 of the waypoint, update the unexplored clusters list in case we happen to have found a new unexplored cluster. 
                            if (updatedUnexploredClusterListAfterMove == false && (nextPoint - ship.GetPosition()).LengthSquared() <  Math.Pow(400, 2))
                            {
                                Log("Updating unexplored cluster list after ship move.");
                                UpdateUnexploredWarpsList();
                                updatedUnexploredClusterListAfterMove = true;
                            }
                        }

                        var newWarps = ship.GetCluster().GetWarps().Where(p => GameInfo.UnexploredClustersByObjectID.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == true && consideredWarpObjectIDs.Contains(p.GetObjectID()) == false);
                        if (newWarps.Count() > 0)
                        {
                            foreach (var newWarp in newWarps)
                            {
                                // If there are other non-sweeping ships in the sector, then only 33% of those ships should go to the newly visible aelph.
                                if (otherScoutShips.Count() - otherSweepingScoutShips.Count() > 0)
                                {
                                    if (_random.Next(0, 100) < 33)
                                    {
                                        Log($"Warp found, 33% chance success, heading to {newWarp.GetName()}");

                                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, newWarp, (sbyte)CommandID.c_cidGoto);
                                        ship.SetAutopilot(true);
                                        foundNewWarp = true;
                                        break;
                                    }
                                }
                                else // We are the only non-sweeping scout in the cluster, go to the new warp.
                                {
                                    Log($"Warp found, heading to {newWarp.GetName()}");

                                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, newWarp, (sbyte)CommandID.c_cidGoto);
                                    ship.SetAutopilot(true);
                                    foundNewWarp = true;
                                    break;
                                }
                            }
                        }

                        Thread.Sleep(100);
                    }

                    if (foundNewWarp == false)
                    {
                        Log("No aelph was found, heading to next available warp.");
                        SelectNextAelphTarget();
                    }
                }
                finally
                {
                    _runningTasks--;
                }
            });
        }

        private void SweepCluster()
        {
            Log("SweepCluster()");

            _isSweeping = true;

            var ship = _client.GetShip();
            var otherScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true);

            var centerPoint = new VectorWrapper(0, 0, 0);
            var shipPosition = ship.GetPosition();

            var startingPoint = shipPosition;
            var startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);

            double nextSliceWidth = (Math.PI * 2 / SweepHopCount);
            double currentAngleInRadians = startingAngleInRadians;

            bool sweepLeft = otherScoutShips.Count() > 0;

            Task.Run(() =>
            {
                try
                {
                    _runningTasks++;

                    bool sweepComplete = true;

                    // Skip the first hop and the last 2 hops, aelphs won't be that close together.
                    for (int i = 1; i < SweepHopCount - 2 && _cancellationTokenSource.IsCancellationRequested == false; i++)
                    {
                        if (sweepLeft == true)
                            currentAngleInRadians += nextSliceWidth;
                        else
                            currentAngleInRadians -= nextSliceWidth;

                        VectorWrapper nextPoint = new VectorWrapper((startingPoint - centerPoint).Length() * (float)Math.Cos(currentAngleInRadians), (startingPoint - centerPoint).Length() * (float)Math.Sin(currentAngleInRadians), 0);

                        Log($"nextPoint: {nextPoint.GetString()}, currentAngleInRadians: {currentAngleInRadians}, sweepHop: {i}");

                        var buoy = _client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
                        var command = ship.GetDefaultOrder(buoy);
                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                        ship.SetAutopilot(true);

                        bool reachedWaypoint = false;
                        for (int j = 0; j < 120 * 100 && _cancellationTokenSource.IsCancellationRequested == false; j++)
                        {
                            VectorWrapper difference = nextPoint - ship.GetPosition();

                            if(j % 500 == 0)
                                Log($"Current distance: {Math.Abs(difference.Length())}, iterations: {j}, {nextPoint.GetString()}");

                            if ((nextPoint - ship.GetPosition()).LengthSquared() < Math.Pow(400, 2))
                            {
                                Log($"Reached waypoint: {nextPoint.GetString()}"); 
                                reachedWaypoint = true;
                                break;
                            }

                            Thread.Sleep(100);
                        }

                        // We didn't reach the waypoint in two minutes. Something went wrong, let's just move on.
                        if (reachedWaypoint == false)
                        {
                            sweepComplete = false;
                            Log("No waypoint was found in 120 seconds.");
                            break;
                        }
                    }

                    if (sweepComplete == true)
                    {
                        Log($"Cluster exploration complete for ship.GetCluster().GetName(), removing from GameInfo.UnexploredClustersByObjectID");
                        GameInfo.UnexploredClustersByObjectID.Remove(ship.GetCluster().GetObjectID());
                        UpdateUnexploredWarpsList();
                    }

                    SelectNextAelphTarget();
                }
                finally
                {
                    _runningTasks--;
                    _isSweeping = false;
                }
            });

            //_startingPoint = shipPosition;
            //_startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);




            //if (_startingAngleInRadians < 0)
            //    _startingAngleInRadians = _startingAngleInRadians + 2 * Math.PI;

            //_currentAngleInRadians = _startingAngleInRadians;

            //// If there are no scouts, sweep left, otherwise if there is already a scount, then sweep right, otherwise, locate a new aleph and go there!
            //if (otherScoutShips.Count() == 0)
            //{
            //    _sweepLeft = true;
            //    NavigateToNextSweepPoint();
            //}
            //else if (otherScoutShips.Count() == 1)
            //{
            //    _sweepRight = true;
            //    NavigateToNextSweepPoint();
            //}
            //else
            //{
            //    _sweepRight = false;
            //    _sweepLeft = false;
            //    SelectNextAelphTarget();
            //}
        }

        //private void NavigateToNextSweepPoint()
        //{
        //    var ship = _client.GetShip();

        //    double nextSliceWidth = (Math.PI * 2 / 16);
        //    double nextSlice = nextSliceWidth;

        //    if (_sweepLeft == true)
        //    {
        //        nextSlice = _currentAngleInRadians + nextSliceWidth;
        //        if (nextSlice > Math.PI * 2)
        //            nextSlice -= Math.PI * 2;
        //    }
        //    else
        //    {
        //        nextSlice = _currentAngleInRadians - nextSliceWidth;
        //        if (nextSlice < 0)
        //            nextSlice += Math.PI * 2;
        //    }

        //    // Don't travel the last slice.
        //    if (_totalRadiansTraversed + nextSliceWidth >= Math.PI * 2)
        //        SelectNextAelphTarget();

        //    _totalRadiansTraversed += nextSliceWidth;

        //    var centerPoint = new VectorWrapper(0, 0, 0);
        //    VectorWrapper nextPoint = new VectorWrapper((_startingPoint - centerPoint).Length() * (float) Math.Cos(nextSlice), (_startingPoint - centerPoint).Length() * (float) Math.Sin(nextSlice), 0);

        //    var buoy = _client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
        //    var command = ship.GetDefaultOrder(buoy);
        //    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
        //    ship.SetAutopilot(true);
        //}

        private void SelectNextAelphTarget()
        {
            Log("SelectNextAelphTarget()");

            var ship = _client.GetShip();
            
            var visibleWarps = ship.GetCluster().GetWarps();
            var otherFriendlyScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true && p.GetSide().GetObjectID() == ship.GetSide().GetObjectID());

            bool foundTargetWarp = false;

            foreach (var visibleWarp in visibleWarps.Where(p => GameInfo.UnexploredClustersByObjectID.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == true).OrderBy(p => _client.GetDistanceSquared(p, ship)))
            {
                if (GameInfo.UnexploredClustersByObjectID.ContainsKey(visibleWarp.GetDestination().GetCluster().GetObjectID()) == true)
                {
                    Log($"Unexplored Warp found: {visibleWarp.GetName()}");

                    float myDistance = _client.GetDistanceSquared(ship, visibleWarp);
                    bool isAnotherScoutCloser = false;
                    foreach (var otherScoutShip in otherFriendlyScoutShips)
                    {
                        float otherScoutDistance = _client.GetDistanceSquared(otherScoutShip, visibleWarp);
                        if (otherScoutDistance < myDistance)
                        {
                            Log($"Another scout: {otherScoutShip.GetName()} is already closer to: {visibleWarp.GetName()}, looking for another target.");
                            isAnotherScoutCloser = true;
                            break;
                        }
                    }

                    // The cluster is on the unexplored list, and there's no other scout that is closer, let's go for it!
                    if (isAnotherScoutCloser == false)
                    {
                        Log($"Found target warp, going to {visibleWarp.GetName()}");

                        foundTargetWarp = true;
                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, visibleWarp, (sbyte)CommandID.c_cidGoto);
                        ship.SetAutopilot(true);
                        break;
                    }
                }
            }

            if (foundTargetWarp == false)
            {
                Log($"No target warp found, selecting a visible warp at random.");

                // Pick a random warp that is unexplored. 
                var unvisitedWarps = visibleWarps.Where(p => GameInfo.UnexploredClustersByObjectID.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == true).ToList();
                if (unvisitedWarps.Count > 0)
                {
                    var targetWarp = unvisitedWarps[_random.Next(0, unvisitedWarps.Count)];

                    Log($"Found unvisited random warp, going to: {targetWarp.GetName()}");

                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetWarp, (sbyte)CommandID.c_cidGoto);
                    ship.SetAutopilot(true);
                    foundTargetWarp = true;
                }
            }

            // Couldn't find any immediately linked clusters, so find the nearest unexplored cluster and go to it.
            if (foundTargetWarp == false)
            {
                IclusterIGCWrapper nearestCluster = FindNearestUnexploredCluster();
                if (nearestCluster != null)
                {
                    Log($"Found an unexplored cluster {nearestCluster.GetName()}, navigating to it.");

                    var buoy = _client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 0, 0, 0, nearestCluster.GetObjectID(), true);
                    var command = ship.GetDefaultOrder(buoy);
                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                    ship.SetAutopilot(true);

                    _navigatingToCluster = nearestCluster;

                    foundTargetWarp = true;
                }
            }

            // All the immediate warps have been visited or have other scouts closer, just pick one at random.
            if (foundTargetWarp == false)
            {
                var targetWarp = visibleWarps[_random.Next(0, visibleWarps.Count)];

                Log($"Found visited random warp, going to: {targetWarp.GetName()}");

                ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetWarp, (sbyte)CommandID.c_cidGoto);
                ship.SetAutopilot(true);
                foundTargetWarp = true;
            }
        }

        private IclusterIGCWrapper FindNearestUnexploredCluster()
        {
            IclusterIGCWrapper nearestCluster = null;
            int nearestClusterDistance = int.MaxValue;

            foreach (var unexploredClusterObjectID in GameInfo.UnexploredClustersByObjectID.Keys)
            {
                var fromCluster = _client.GetShip().GetCluster();
                var toCluster = _client.GetCore().GetCluster((short)unexploredClusterObjectID);

                DijkstraPathFinder pathFinder = new DijkstraPathFinder(_client.GetCore(), fromCluster, toCluster);
                
                int distance = pathFinder.GetDistance(fromCluster, toCluster);

                if (distance < nearestClusterDistance)
                    nearestCluster = toCluster;
            }

            return nearestCluster;
        }
        

        private void MessageReceiver_FMD_S_SINGLE_SHIP_UPDATE(ClientConnection client, AllegianceInterop.FMD_S_SINGLE_SHIP_UPDATE message)
        {
            
        }

        private void MessageReceiver_FMD_S_SET_CLUSTER(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_CLUSTER message)
        {
            
        }

        public override void Start()
        {
            var ship = _client.GetShip();
            var side = _client.GetSide();
            var mission = side.GetMission();
            var hullTypes = mission.GetHullTypes();

            var station = ship.GetStation();

            // If we are in a station, then get a scout ready
            if (station != null)
                ChangeShip(ShipType.Scout, new ScoutProbeLoadout());

            if (IsCurrentShipAScount() == false)
                ReturnToBase();

            //else
            //    ScoutSector();
            
        }

        private void ScoutSector()
        {
            var ship = _client.GetShip();

            // If the ship is in a station, launch.
            if (ship.GetStation() != null)
            {
                // TODO: Transfer to desired station first.

                // Launch the ship
                //_client.BuyLoadout(ship, true);

                // Wait for the ship to enter the cluster.
                while (ship.GetCluster() == null)
                    Thread.Sleep(100);
            }

            var mission = ship.GetMission();

            // Get all the clusters that are available in the mission.
            var missionClusters = mission.GetClusters();

            var currentShipCluster = ship.GetCluster();
            var currentMissionCluster = missionClusters.FirstOrDefault(p => p.GetObjectID() == currentShipCluster.GetObjectID());

            var shipVisibleWarps = currentShipCluster.GetWarps();
            var allWarpsInCluster = currentMissionCluster.GetWarps();
        }

        private void ReturnToBase()
        {
            var ship = _client.GetShip();
            var targetBase = _client.FindTarget(ship, (int) (TargetType.c_ttStation | TargetType.c_ttFriendly | TargetType.c_ttAnyCluster), (short) StationAbilityBitMask.c_sabmFlag);

            ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetBase, (sbyte)CommandID.c_cidGoto);
            ship.SetAutopilot(true);
        }

        private bool IsCurrentShipAScount()
        {
            var ship = _client.GetShip();
            var baseHullType = ship.GetBaseHullType();

            return baseHullType.GetName().Contains("Scout");
        }

        private void ChangeShip(ShipType shipType, LoadoutBase loadOut)
        {
            var side = _client.GetSide();
            var ship = _client.GetShip();
            var mission = side.GetMission();
            var hullTypes = mission.GetHullTypes();
            var station = ship.GetStation();

            if (station == null)
                return;

            string targetHullTypeName = GetHullTypeString(shipType);
            
            List<AllegianceInterop.IhullTypeIGCWrapper> buyableHullTypes = new List<AllegianceInterop.IhullTypeIGCWrapper>();

            foreach (var hullType in hullTypes)
            {
                if (hullType.GetGroupID() >= 0 && station.CanBuy(hullType) == true && station.IsObsolete(hullType) == false)
                {
                    Log("buyable hullType: " + hullType.GetName());
                    buyableHullTypes.Add(hullType);
                }
            }
            
            // Get a scout.
            var scoutHull = buyableHullTypes.FirstOrDefault(p => p.GetName().Contains(targetHullTypeName));
            if (scoutHull == null)
                return;

            Log($"Found Scout: {scoutHull.GetName()}");

            var partTypes = mission.GetPartTypes();
            List<AllegianceInterop.IpartTypeIGCWrapper> buyablePartTypes = new List<AllegianceInterop.IpartTypeIGCWrapper>();
            foreach (var partType in partTypes)
            {
                if (partType.GetGroupID() >= 0 && station.CanBuy(partType) == true && station.IsObsolete(partType) == false)
                {
                    short equipmentTypeID = partType.GetEquipmentType();

                    bool isIncluded = false;

                    switch ((EquipmentType)equipmentTypeID)
                    {
                        case EquipmentType.NA:
                            isIncluded = true;
                            break;

                        case EquipmentType.ET_Weapon:
                        case EquipmentType.ET_Turret:
                            for (sbyte i = 0; i < scoutHull.GetMaxFixedWeapons(); i++)
                            {
                                isIncluded |= scoutHull.GetPartMask(equipmentTypeID, i) != 0 && scoutHull.CanMount(partType, i) == true;
                            }
                            break;

                        default:
                            isIncluded |= scoutHull.GetPartMask(equipmentTypeID, 0) != 0 && scoutHull.CanMount(partType, 0) == true;

                            break;
                    }

                    if (isIncluded == true)
                    {
                        buyablePartTypes.Add(partType);
                        Log($"\tFound part: {partType.GetName()}, capacity for part: {scoutHull.GetCapacity(partType.GetEquipmentType())}");

                    }
                }
            }

            // Change the ship to the scout hull. 
            ship.SetBaseHullType(scoutHull);
            _client.BuyDefaultLoadout(ship, station, scoutHull, _client.GetMoney());

            // Clear the cargo.
            for (sbyte i = -5; i < 0; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.NA, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear weapons
            for (sbyte i = 0; i < 4; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Weapon, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear turrets
            for (sbyte i = 0; i < ship.GetHullType().GetMaxWeapons() - ship.GetHullType().GetMaxFixedWeapons(); i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Turret, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear missiles
            for (sbyte i = 0; i < 1; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Magazine, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear Dispenser
            for (sbyte i = 0; i < 1; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Dispenser, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }


            // Load Weapons, missiles, dispensers, etc.
            if (loadOut.Weapon1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon1.Value)) == true);
                if(part != null)
                    _client.BuyPartOnBudget(ship, part, 0, _client.GetMoney());
            }

            if (loadOut.Weapon2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon2.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 1, _client.GetMoney());
            }

            if (loadOut.Weapon3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon3.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 2, _client.GetMoney());
            }

            if (loadOut.Weapon4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon4.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 3, _client.GetMoney());
            }

            if (loadOut.Turret1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret1.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 0, _client.GetMoney());
            }

            if (loadOut.Turret2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret2.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 1, _client.GetMoney());
            }

            if (loadOut.Turret3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret3.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 2, _client.GetMoney());
            }

            if (loadOut.Turret4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret4.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 3, _client.GetMoney());
            }

            if (loadOut.Dispenser != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Dispenser.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 0, _client.GetMoney());
            }

            if (loadOut.Missiles != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Missiles.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, 0, _client.GetMoney());
            }

            if (loadOut.Cargo1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo1.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, -1, _client.GetMoney());
            }

            if (loadOut.Cargo2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo2.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, -2, _client.GetMoney());
            }

            if (loadOut.Cargo3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo3.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, -3, _client.GetMoney());
            }

            if (loadOut.Cargo4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo4.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, -4, _client.GetMoney());
            }

            if (loadOut.Cargo5 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo5.Value)) == true);
                if (part != null)
                    _client.BuyPartOnBudget(ship, part, -5, _client.GetMoney());
            }

            
            // Launch! 
            _client.BuyLoadout(ship, true);
        }

        private string GetHullTypeString(ShipType shipType)
        {
            switch (shipType)
            {
                case ShipType.Scout:
                    return "Scout";

                case ShipType.Fighter:
                    return "Fighter";

                case ShipType.Interceptor:
                    return "Interceptor";

                case ShipType.Bomber:
                    return "Bomber";

                case ShipType.Gunship:
                    return "Gunship";

                case ShipType.StealthFighter:
                    return "Stealth Fighter";

                default:
                    throw new NotSupportedException(shipType.ToString());
            }
        }
    }
}
