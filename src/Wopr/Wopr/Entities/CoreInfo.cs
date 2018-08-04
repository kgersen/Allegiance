using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Entities
{
    public class CoreInfo : MessageElement
    {
        public string Name; // c_cbName = 25
        public bool bOfficial;
        public int dwBit;
        public StaticCoreInfo mStatic;

        public static CoreInfo Read(BinaryReader br)
        {
            CoreInfo returnValue = new CoreInfo();

            returnValue.Name = ReadString(br, 25);
            returnValue.bOfficial = br.ReadByte() > 0x00;
            returnValue.dwBit = br.ReadInt32();
            returnValue.mStatic = StaticCoreInfo.Read(br);

            return returnValue;
        }
    }
}
