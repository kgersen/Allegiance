using AllegianceInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Entities
{
    /// <summary>
    /// This object is shared between all running strategies on a team. It's used to communicate state between bots.
    /// </summary>
    [Serializable]
    public class GameInfo
    {
        // The game server will give us a list of all clusters and warps, but not their locations at the beginning of the game so that bots will
        // know when they have explored the map. This object contains all warps and clusters in the game regardless of if we have eyed them or not.
        public List<ClusterInfo> Clusters { get; set; }

        // When a bot detects a new warp, it will add it to this list. When a bot completes a exploration sweep, it will remove the warp from the list.
        //public Dictionary<int, String> UnexploredClustersByObjectID { get; set; } = new Dictionary<int, String>()

        // When a bot starts sweeping a sector, it will increment the sweep count for the sector. When it is finished, it will decrement the counter.
        public Dictionary<int, int> SweepingScoutCountByClusterObjectID { get; set; } = new Dictionary<int, int>();

        public int GetSweepingScoutCount(IclusterIGCWrapper cluster)
        {
            int sweepingScoutCount;
            if (SweepingScoutCountByClusterObjectID.TryGetValue(cluster.GetObjectID(), out sweepingScoutCount) == true)
                return sweepingScoutCount;

            return 0;
        }

        public int IncrementSweepingScoutCount(IclusterIGCWrapper cluster)
        {
            lock (SweepingScoutCountByClusterObjectID)
            {
                int sweepingScoutCount;
                if (SweepingScoutCountByClusterObjectID.TryGetValue(cluster.GetObjectID(), out sweepingScoutCount) == true)
                    SweepingScoutCountByClusterObjectID[cluster.GetObjectID()]++;
                else
                    SweepingScoutCountByClusterObjectID.Add(cluster.GetObjectID(), 1);

                return SweepingScoutCountByClusterObjectID[cluster.GetObjectID()];
            }
        }

        public void DecrementSweepingScoutCount(IclusterIGCWrapper cluster)
        {
            lock (SweepingScoutCountByClusterObjectID)
            {
                int sweepingScoutCount;
                if (SweepingScoutCountByClusterObjectID.TryGetValue(cluster.GetObjectID(), out sweepingScoutCount) == true)
                {
                    if (sweepingScoutCount <= 1)
                        SweepingScoutCountByClusterObjectID[cluster.GetObjectID()] = 0;
                    else
                        SweepingScoutCountByClusterObjectID[cluster.GetObjectID()]--;
                }
                else
                {
                    SweepingScoutCountByClusterObjectID.Add(cluster.GetObjectID(), 0);
                }
            }
        }

        public Dictionary<int, string> GetUnexploredClusters(ImissionIGCWrapper mission)
        {
            Dictionary<int, string> returnValue = new Dictionary<int, string>();

            foreach (var clusterInfo in Clusters)
            {
                var missionCluster = mission.GetCluster(clusterInfo.GetObjectID());

                // Did we find all the warps?
                if (missionCluster.GetWarps().Count < clusterInfo.GetWarps().Count)
                {
                    returnValue.Add(clusterInfo.GetObjectID(), missionCluster.GetName());
                }
                // Did we find the tech rock?
                else if (missionCluster.GetAsteroids().Where(p => p.GetName().StartsWith("a") == false && p.GetName().StartsWith("He") == false).Count() == 0)
                {
                    returnValue.Add(clusterInfo.GetObjectID(), missionCluster.GetName());
                }
            }


            return returnValue;
        }

    }
}
