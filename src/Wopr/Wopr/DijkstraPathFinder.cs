using AllegianceInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr
{
    public class IclusterIGCWrapperComparer : IEqualityComparer<IclusterIGCWrapper>
    {
        public bool Equals(IclusterIGCWrapper x, IclusterIGCWrapper y)
        {
            return x.GetObjectID() == y.GetObjectID();
        }

        public int GetHashCode(IclusterIGCWrapper obj)
        {
            return obj.GetObjectID().GetHashCode();
        }
    }

    public class DijkstraPathFinder
    {
        //Dictionary<IclusterIGCWrapper, IclusterIGCWrapper> _predecessor = new Dictionary<IclusterIGCWrapper, IclusterIGCWrapper>(new IclusterIGCWrapperComparer());
        //Dictionary<IclusterIGCWrapper, int> _estimate = new Dictionary<IclusterIGCWrapper, int>(new IclusterIGCWrapperComparer());
        //Dictionary<IclusterIGCWrapper, String> _queue = new Dictionary<IclusterIGCWrapper, string>(new IclusterIGCWrapperComparer());

        Dictionary<int?, int?> _predecessor = new Dictionary<int?, int?>();
        Dictionary<int?, int?> _estimate = new Dictionary<int?, int?>();
        Dictionary<int?, String> _queue = new Dictionary<int?, string>();


        public DijkstraPathFinder(List<ClusterInfo> clusterInfos, int fromClusterObjectID, int toClusterObjectID)
        {
            BuildPath(clusterInfos, fromClusterObjectID, toClusterObjectID);
        }

        public DijkstraPathFinder(ImissionIGCWrapper mission, IclusterIGCWrapper fromCluster, IclusterIGCWrapper toCluster)
        {
            List<ClusterInfo> clusterInfos = new List<ClusterInfo>();

            foreach (var cluster in mission.GetClusters())
            {
                clusterInfos.Add(new ClusterInfo(cluster.GetObjectID(), cluster.GetHomeSector()));
            }

            foreach (var fromClusterInfo in clusterInfos)
            {
                foreach (var warp in mission.GetCluster(fromClusterInfo.GetObjectID()).GetWarps())
                {
                    var toClusterInfo = clusterInfos.Where(p => p.GetObjectID() == warp.GetDestination().GetCluster().GetObjectID()).FirstOrDefault();

                    if (toClusterInfo != null)
                        fromClusterInfo.GetWarps().Add(new WarpInfo(fromClusterInfo, toClusterInfo));
                }
            }

            BuildPath(clusterInfos, fromCluster.GetObjectID(), toCluster.GetObjectID());
        }

        private void BuildPath(List<ClusterInfo> clusterInfos, int fromClusterObjectID, int toClusterObjectID)
        {
            _predecessor.Clear();
            _queue.Clear();
            _estimate.Clear();

            foreach (var clusterInfo in clusterInfos)
            {
                _predecessor.Add(clusterInfo.GetObjectID(), null);
                _estimate.Add(clusterInfo.GetObjectID(), 1000000);
                _queue.Add(clusterInfo.GetObjectID(), "*");
            }

            _estimate[fromClusterObjectID] = 0;

            while (_queue.Count > 0)
            {
                int? minimumCluster = MinimumCluster();
                _queue.Remove(minimumCluster);

                var warps = clusterInfos.First(p => p.GetObjectID() == minimumCluster).GetWarps();

                if (warps.Count < 1)
                    continue;

                foreach (var warp in warps)
                {
                    var destinationCluster = warp.GetDestinationCluster().GetObjectID();
                    Relax(minimumCluster, destinationCluster);
                }
            }
        }

        private void Relax(int? fromCluster, int? destinationCluster)
        {
            int weight = 1;
            if (_estimate[destinationCluster] > (_estimate[fromCluster] + weight))
            {
                _estimate[destinationCluster] = _estimate[fromCluster] + weight;
                _predecessor[destinationCluster] = fromCluster;
            }
        }

        private int? MinimumCluster()
        {
            int? minimumCluster = null;
            int? minimumEstimate = 100000000;

            foreach (var queuedCluster in _queue.Keys)
            {
                if (_estimate[queuedCluster] < minimumEstimate)
                {
                    minimumCluster = queuedCluster;
                    minimumEstimate = _estimate[queuedCluster];
                }
            }

            return minimumCluster;
        }

        public int GetDistance(IclusterIGCWrapper fromCluster, IclusterIGCWrapper toCluster)
        {
            return GetDistance(fromCluster.GetObjectID(), toCluster.GetObjectID());
        }

        public int GetDistance(int fromClusterObjectID, int toClusterObjectID)
        {
            int i = 0;
            for (int? currentCluster = toClusterObjectID; currentCluster != fromClusterObjectID && currentCluster != null; currentCluster = _predecessor[currentCluster])
                i++;

            return i;
        }

        public int? NextClusterObjectID(int fromClusterObjectID, int toClusterObjectID)
        {
            int? returnValue = null;

            for (int? currentCluster = toClusterObjectID; currentCluster != fromClusterObjectID && currentCluster != null; currentCluster = _predecessor[currentCluster])
                returnValue = currentCluster;

            return returnValue;
        }
    }
}
