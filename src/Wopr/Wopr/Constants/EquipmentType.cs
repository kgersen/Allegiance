using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public enum EquipmentType : short
    {
        ET_Turret = -2,
        NA = -1,
        ET_ChaffLauncher = 0,
        ET_Weapon = 1,
        ET_Magazine = 2,
        ET_Dispenser = 3,
        ET_Shield = 4,
        ET_Cloak = 5,
        ET_Pack = 6,
        ET_Afterburner = 7,
        ET_MAX = 8
    }
}
