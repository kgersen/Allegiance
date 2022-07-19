using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strategies.Loadouts
{
    public class ScoutProbeLoadout : LoadoutBase
    {
        public ScoutProbeLoadout()
        {
            Weapon1 = Wopr.Constants.ItemType.Gatling;

            Missiles = Wopr.Constants.ItemType.MrmMissile;

            Dispenser = Wopr.Constants.ItemType.EwsProbe;

            Cargo1 = Wopr.Constants.ItemType.EwsProbe;
            Cargo2 = Wopr.Constants.ItemType.EwsProbe;
            Cargo3 = Wopr.Constants.ItemType.EwsProbe;
            Cargo4 = Wopr.Constants.ItemType.EwsProbe;
            Cargo5 = Wopr.Constants.ItemType.Nanite;
        }
    }
}
