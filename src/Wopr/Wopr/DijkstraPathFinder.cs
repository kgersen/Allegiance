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
        Dictionary<IclusterIGCWrapper, IclusterIGCWrapper> _predecessor = new Dictionary<IclusterIGCWrapper, IclusterIGCWrapper>(new IclusterIGCWrapperComparer());
        Dictionary<IclusterIGCWrapper, int> _estimate = new Dictionary<IclusterIGCWrapper, int>(new IclusterIGCWrapperComparer());
        Dictionary<IclusterIGCWrapper, String> _queue = new Dictionary<IclusterIGCWrapper, string>(new IclusterIGCWrapperComparer());
       

        public DijkstraPathFinder(ImissionIGCWrapper mission, IclusterIGCWrapper fromCluster, IclusterIGCWrapper toCluster)
        {
            _predecessor.Clear();
            _queue.Clear();
            _estimate.Clear();

            foreach (var cluster in mission.GetClusters())
            {
                _predecessor.Add(cluster, null);
                _estimate.Add(cluster, 1000000);
                _queue.Add(cluster, "*");
            }

            _estimate[fromCluster] = 0;

            while (_queue.Count > 0)
            {
                IclusterIGCWrapper minimumCluster = MinimumCluster();
                _queue.Remove(minimumCluster);

                var warps = minimumCluster.GetWarps();

                if (warps.Count < 1)
                    continue;

                foreach (var warp in warps)
                {
                    var destinationCluster = warp.GetDestination().GetCluster();
                    Relax(minimumCluster, destinationCluster);
                }
            }
        }

        private void Relax(IclusterIGCWrapper fromCluster, IclusterIGCWrapper destinationCluster)
        {
            int weight = 1;
            if (_estimate[destinationCluster] > (_estimate[fromCluster] + weight))
            {
                _estimate[destinationCluster] = _estimate[fromCluster] + weight;
                _predecessor[destinationCluster] = fromCluster;
            }
        }

        private IclusterIGCWrapper MinimumCluster()
        {
            IclusterIGCWrapper minimumCluster = null;
            int minimumEstimate = 100000000;

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
            int i = 0;
            for (var currentCluster = toCluster; currentCluster.GetObjectID() != fromCluster.GetObjectID() && currentCluster != null; currentCluster = _predecessor[currentCluster])
                i++;

            return i;
        }

        public IclusterIGCWrapper NextCluster(IclusterIGCWrapper fromCluster, IclusterIGCWrapper toCluster)
        {
            IclusterIGCWrapper returnValue = null;

            for (var currentCluster = toCluster; currentCluster.GetObjectID() != fromCluster.GetObjectID() && currentCluster != null; currentCluster = _predecessor[currentCluster])
                returnValue = currentCluster;

            return returnValue;
        }
    }
}
