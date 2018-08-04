using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Wopr.Entities
{
    public class MessageElement
    {
        //public static Int32 ReadInt32(BinaryReader br)
        //{
        //    uint value = br.ReadUInt32();

        //    return (int) ((value & 0x000000FF) << 24 | (value & 0x0000FF00) << 8 |
        //            (value & 0x00FF0000) >> 8 | (value & 0xFF000000) >> 24);

        //}

        public static String ReadString(BinaryReader br, int maxLength)
        {
            byte[] chars = br.ReadBytes(maxLength);
            return System.Text.ASCIIEncoding.ASCII.GetString(chars).Split('\0')[0];
        }
    }
}
