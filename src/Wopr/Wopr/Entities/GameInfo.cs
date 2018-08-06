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
        // When a bot detects a new warp, it will add it to this list. When a bot completes a exploration sweep, it will remove the warp from the list.
        public Dictionary<int, String> UnexploredClustersByObjectID { get; set; }
    }
}
