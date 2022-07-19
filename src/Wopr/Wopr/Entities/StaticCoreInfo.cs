using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Wopr.Entities
{
    public class StaticCoreInfo : MessageElement
    {
        public string cbIGCFile; // c_cbFileName = 13

        internal static StaticCoreInfo Read(BinaryReader br)
        {
            StaticCoreInfo returnValue = new StaticCoreInfo();

            returnValue.cbIGCFile = ReadString(br, 13);

            return returnValue;
        }
    };
}
