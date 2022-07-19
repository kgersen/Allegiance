using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Entities
{
    public struct PersistPlayerScoreObject : ISupportReadingFromStreams
    {
        public float m_totalScore;
        public float m_fCombatRating;
        public short m_rank;
        public short m_civID;

        public int GetByteSizeOfEntity()
        {
            return System.Runtime.InteropServices.Marshal.SizeOf(new PersistPlayerScoreObject());
        }

        public void Read(BinaryReader br)
        {
            m_totalScore = System.BitConverter.ToSingle(br.ReadBytes(4), 0);
            m_fCombatRating = System.BitConverter.ToSingle(br.ReadBytes(4), 0);
            m_rank = br.ReadInt16();
            m_civID = br.ReadInt16();
        }
    }
}
