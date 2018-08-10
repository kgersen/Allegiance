using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wopr.Constants;

namespace Wopr.Loadouts
{
    public abstract class LoadoutBase
    {
        public ItemType? Weapon1 = null;
        public ItemType? Weapon2 = null;
        public ItemType? Weapon3 = null;
        public ItemType? Weapon4 = null;
                       
        public ItemType? Missiles = null;

        public ItemType? Dispenser = null;

        public ItemType? Cargo1 = null;
        public ItemType? Cargo2 = null;
        public ItemType? Cargo3 = null;
        public ItemType? Cargo4 = null;
        public ItemType? Cargo5 = null;
                       
        public ItemType? Turret1 = null;
        public ItemType? Turret2 = null;
        public ItemType? Turret3 = null;
        public ItemType? Turret4 = null;

        public string GetItemTypeString(ItemType itemType)
        {
            switch (itemType)
            {
                case ItemType.Ammunition:
                    return "Ammunition";

                case ItemType.Counter:
                    return "Counter 1";

                case ItemType.EwsProbe:
                    return "EWS Probe";

                case ItemType.Fuel:
                    return "Fuel";

                case ItemType.Gatling:
                    return "PW Gat Gun";

                case ItemType.MrmMissile:
                    return "MRM Seeker";

                case ItemType.Nanite:
                    return "ER Nanite";

                case ItemType.ProxMine:
                    return "Prox Mine";

                default:
                    throw new NotSupportedException(itemType.ToString());
            }
        }
    }
}
