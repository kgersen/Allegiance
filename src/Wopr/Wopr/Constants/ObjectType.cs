using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public enum ObjectType
    {
        OT_invalid = -1,

        OT_modelBegin = 0,
        OT_ship = 0,                //All classes derived from model
        OT_station = 1,
        OT_missile = 2,
        OT_mine = 3,
        OT_probe = 4,
        OT_asteroid = 5,

        OT_projectile = 6,
        OT_warp = 7,
        OT_treasure = 8,           //All of the above are models
        OT_buoy = 9,
        OT_chaff = 10,
        OT_buildingEffect = 11,
        OT_modelEnd = 11,

        OT_side = 12,
        OT_cluster = 13,
        OT_bucket = 14,

        OT_partBegin = 15,
        OT_weapon = 15,
        OT_shield = 16,
        OT_cloak = 17,
        OT_pack = 18,
        OT_afterburner = 19,
        OT_launcherBegin = 20,
        OT_magazine = 20,
        OT_dispenser = 21,
        OT_launcherEnd = 21,
        OT_partEnd = 21,


        OT_staticBegin = 22,
        OT_projectileType = 22,      //All static objects
        OT_missileType = 23,
        OT_mineType = 24,
        OT_probeType = 25,
        OT_chaffType = 26,
        OT_civilization = 27,
        OT_treasureSet = 28,

        OT_bucketStart = 29,
        OT_hullType = 29,
        OT_partType = 30,
        OT_stationType = 31,
        OT_development = 32,
        OT_droneType = 33,
        OT_bucketEnd = 33,
        OT_staticEnd = 33,

        OT_constants = 34,
        OT_allsrvUser = 35, // used by the admin object model
        OT_Max = 36,// don't put anything after this
                    // OT_Max should be less then 256 for
                    // AGC event firing.
    }
}
