using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public enum ShipState
    {
        c_ssDead = 1,                //A nice enumeration of all the things it could have been doing
        c_ssDocked = 2,              //when it was last seen (if only briefly)
        c_ssFlying = 3,
        c_ssObserver = 4,
        c_ssTurret = 5
    }
}
