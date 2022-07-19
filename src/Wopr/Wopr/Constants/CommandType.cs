using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public enum CommandType
    {
        c_cmdQueued = 0,
        c_cmdAccepted = 1,
        c_cmdCurrent = 2,
        c_cmdPlan = 3,
        c_cmdMax = 4
    }
}
