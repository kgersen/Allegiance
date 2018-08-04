using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr.Constants
{
    public class BitReader
    {
        private int _bitOffset;
        //private BitArray _bitArray;
        List<Int32> _int32DataBuckets = null;
        List<Byte> _byteDataBuckets = null;
        private int _currentDataBucket;
        //private int _readBitCounter;
        private double _alignmentSizeInBytes;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="binaryReader"></param>
        /// <param name="bitLength">The number of bits that will be read from the stream. Padding will be added to ensure that the alignment of the c struct is preserved.</param>
        /// <param name="alignmentSizeInBytes">normally the c structs will be working with will always be four byte containers (int / uint will be the max size)</param>
        private BitReader()
        {
        }

        public static BitReader CreateUsingInt32Containers(BinaryReader binaryReader, double bitLength)
        {
            BitReader returnValue = new BitReader();

            double byteSize = 8; // 8 bits in a byte

            returnValue._alignmentSizeInBytes = System.Runtime.InteropServices.Marshal.SizeOf(typeof(Int32));

            double bitArrayByteCount = bitLength / byteSize; // number of ints needed to hold bit based data. 
            if (bitArrayByteCount / returnValue._alignmentSizeInBytes != Math.Floor(bitArrayByteCount / returnValue._alignmentSizeInBytes))
                bitArrayByteCount = Math.Floor(bitArrayByteCount) + returnValue._alignmentSizeInBytes; // Add a the alignment pad to match c++ struct alignment.

            
            returnValue._int32DataBuckets = new List<int>();

            for (int i = 0; i < bitArrayByteCount / 4; i++)
            {
                returnValue._int32DataBuckets.Add(binaryReader.ReadInt32());
            }

            returnValue._currentDataBucket = 0;
            //returnValue._readBitCounter = 0;
            
            return returnValue;
        }

        public static BitReader CreateUsingByteContainers(BinaryReader binaryReader, double bitLength)
        {
            BitReader returnValue = new BitReader();

            double byteSize = 8; // 8 bits in a byte

            returnValue._alignmentSizeInBytes = System.Runtime.InteropServices.Marshal.SizeOf(typeof(Byte));

            double bitArrayByteCount = bitLength / byteSize; // number of ints needed to hold bit based data. 
            if (bitArrayByteCount / returnValue._alignmentSizeInBytes != Math.Floor(bitArrayByteCount / returnValue._alignmentSizeInBytes))
                bitArrayByteCount = Math.Floor(bitArrayByteCount) + returnValue._alignmentSizeInBytes; // Add a the alignment pad to match c++ struct alignment.


            returnValue._byteDataBuckets = new List<Byte>();

            for (int i = 0; i < bitArrayByteCount; i++)
            {
                returnValue._byteDataBuckets.Add(binaryReader.ReadByte());
            }

            returnValue._currentDataBucket = 0;
            //returnValue._readBitCounter = 0;

            return returnValue;
        }

        public uint ReadUInt32(int bitCount)
        {
            uint returnValue = 0;

            if (bitCount + _bitOffset > _alignmentSizeInBytes * 8)
            {
                _bitOffset = 0;
                _currentDataBucket++;
            }

            uint byteMask = 0xFFFFFFFFU >> (32 - bitCount); // (uint)Math.Pow(2, bitCount - 1);

            uint totalValue = (uint) _int32DataBuckets[_currentDataBucket];
            totalValue = totalValue >> _bitOffset;
            _bitOffset += bitCount;

            returnValue = totalValue & byteMask;

            return returnValue;

            //uint returnValue = 0;


            //uint baseValue = (uint) Math.Pow(2, bitCount);

            //for (int i = 0; i < bitCount; i++)
            //{
            //    returnValue = returnValue >> 1;

            //    if (_bitArray.Get(_bitOffset + i) == true)
            //        returnValue |= baseValue;
            //}

            //_bitOffset += bitCount;

            //return returnValue;
        }

        public bool ReadBool()
        {
            return ReadUInt32(1) == 0x01;
        }

        public short ReadShort()
        {
            return (short) ReadUInt32(16);
        }

        public float ReadFloat()
        {
            return BitConverter.ToSingle(BitConverter.GetBytes(ReadUInt32(32)), 0);

        }

        public byte ReadByte()
        {
            return (byte)ReadUInt32(8);
        }
    }
}
