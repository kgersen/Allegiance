using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public enum CommandID
    {
        c_cidDoNothing = -3,
        c_cidMyAccepted = -2,
        c_cidNone = -1,
        c_cidDefault = 0,
        c_cidAttack = 1,
        c_cidCapture = 2,
        c_cidDefend = 3,
        c_cidPickup = 4,
        c_cidGoto = 5,
        c_cidRepair = 6,
        c_cidJoin = 7,
        c_cidMine = 8,
        c_cidBuild = 9,

        c_cidMax = 10
    }
}
