using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    // From igc.h
    public enum TargetType
    {
        c_ttSpecial = 0x80000,        //Special hack for commands
        c_ttMyQueued = 0x80000,        //Low byte must agree with c_cmd
        c_ttMyAccepted = 0x80001,
        c_ttMyTarget = 0x80002,
        c_ttWorstEnemy = 0x80003,
        c_ttMe = 0x80004,

        c_ttFriendly = 0x0001,
        c_ttEnemy = 0x0002,
        c_ttNeutral = 0x0004,

        c_ttShip = 0x0008,
        c_ttStation = 0x0010,
        c_ttAsteroid = 0x0020,
        c_ttTreasure = 0x0040,
        c_ttMissile = 0x0080,
        c_ttWarp = 0x0100,
        c_ttBuoy = 0x0200,

        c_ttFront = 0x0400,    //these two are exclusive
        c_ttNearest = 0x0800,

        c_ttAnyCluster = 0x1000,
        c_ttPrevious = 0x2000,

        c_ttMine = 0x4000,
        c_ttProbe = 0x8000,

        c_ttLeastTargeted = 0x10000,
        c_ttNoRipcord = 0x20000,
        c_ttCowardly = 0x40000,

        c_ttShipTypes = c_ttShip | c_ttStation,

        c_ttAllTypes = c_ttShip | c_ttStation |
                          c_ttAsteroid | c_ttBuoy |
                          c_ttWarp | c_ttTreasure | c_ttMissile | c_ttMine | c_ttProbe,

        c_ttAllSides = c_ttFriendly | c_ttEnemy | c_ttNeutral,

        c_ttAll = c_ttAllSides | c_ttAllTypes
    }
}
