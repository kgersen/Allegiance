using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    // From igc.h

    [Flags]
    public enum StationAbilityBitMask
    {
        c_sabmUnload                = 0x01,      //           Ability to offload mined minerals
        c_sabmStart                 = 0x02,      //           start the game at this station
        c_sabmRestart               = 0x04,      //           restart after dying
        c_sabmRipcord               = 0x08,      //           teleport to the station
        c_sabmCapture               = 0x10,      //           be captured
        c_sabmLand                  = 0x20,      //           land at
        c_sabmRepair                = 0x40,      //           get repaired
        c_sabmRemoteLeadIndicator   = 0x80,      //           relay lead indicator
        c_sabmReload                = 0x100,     //           free fuel and ammo on launch
        c_sabmFlag                  = 0x200,     //           counts for victory
        c_sabmPedestal              = 0x400,     //           be a pedestal for a flag
        c_sabmTeleportUnload        = 0x800,     //           offload mined materials without docking
        c_sabmCapLand               = 0x1000,    //           land capital ships
        c_sabmRescue                = 0x2000,    //           rescue pods
        c_sabmRescueAny             = 0x4000,    //           not used (but reserved for pods)
    }
}
