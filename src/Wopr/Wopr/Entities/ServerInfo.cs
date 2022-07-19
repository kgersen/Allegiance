using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Wopr.Entities
{
    public class ServerInfo
    {
        public ServerCoreInfo mStatic;
        public int ping;
        public bool bOfficial;

        public static ServerInfo Read(BinaryReader br)
        {
            ServerInfo returnValue = new ServerInfo();

            returnValue.mStatic = ServerCoreInfo.Read(br);
            returnValue.ping = br.ReadInt32();
            returnValue.bOfficial = br.ReadByte() > 0x00;

            return returnValue;
        }
    }
}
