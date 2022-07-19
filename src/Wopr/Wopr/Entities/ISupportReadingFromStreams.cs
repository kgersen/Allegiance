using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Entities
{
    public interface ISupportReadingFromStreams
    {
        void Read(BinaryReader br);

        int GetByteSizeOfEntity();
    }
}
