using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Wopr.Entities
{
    public class ServerCoreInfo : MessageElement
    {
        public string szName; // c_cbName = 25
        public string szRemoteAddress; // 64
        public string szLocation; // c_cbFileName = 13
        public int iCurGames;
        public int iMaxGames;
        public int dwCoreMask; // 32 bits mask (so max is 32 cores)

        internal static ServerCoreInfo Read(BinaryReader br)
        {
            ServerCoreInfo returnValue = new ServerCoreInfo();

            returnValue.szName = ReadString(br, 25);
            returnValue.szRemoteAddress = ReadString(br, 64);
            returnValue.szLocation = ReadString(br, 13);

            var junk = br.ReadInt16();

            returnValue.iCurGames = br.ReadInt32();
            returnValue.iMaxGames = br.ReadInt32();
            returnValue.dwCoreMask = br.ReadInt32();

            return returnValue;
        }
    };
}
