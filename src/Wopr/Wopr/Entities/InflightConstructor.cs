using AllegianceInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wopr.Constants;

namespace Wopr.Entities
{
    public class InflightConstructor
    {
        public IclusterIGCWrapper Cluster;
        public IshipIGCWrapper Ship;
        public StationType StationType;
    }
}
