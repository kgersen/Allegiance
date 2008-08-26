//////////////////////////////////////////////////////////////////////////////
//
// Engine Application
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

typedef unsigned int uint;

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class Span {
public:
    BYTE* m_pbyte;
    int   m_length;

    Span(BYTE* pbyte, int length) :
        m_pbyte(pbyte),
        m_length(length)
    {
    }
};

class AllocSpan : public Span {
public:
    AllocSpan(int length) :
        Span(new BYTE[length], length)
    {
    }

    ~AllocSpan()
    {
        delete m_pbyte;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Stream : public IObject {
public:
    virtual void Write(const void* p, uint length) = 0;

    template<class Type> 
    void Write(const Type& value) 
    {
        Write(&value, sizeof(Type));
    }

    template<class Type> 
    void WriteMultiple(const Type& value, uint length)
    {
        for (uint index = 0; index < length; index++) {
            Write(value);
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class MemoryStream : public Stream {
private:
    TVector<BYTE> m_vb;

public:
    MemoryStream()
    {
    }

    void Write(const void* p, uint length)
    {
        int count = m_vb.GetCount();

        m_vb.SetCount(count + length);

        memcpy(&(m_vb.Get(count)), p, length);
    }

    template<class Type> 
    void Write(const Type& value) 
    {
        Write(&value, sizeof(Type));
    }

    Span GetSpan()
    {
        return Span((BYTE*)&(m_vb[0]), m_vb.GetCount());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
void Extract(BYTE*& pbyte, Type& value)
{
    value  = *((Type*)pbyte);
    pbyte += sizeof(Type);
}

//////////////////////////////////////////////////////////////////////////////
//
// RLECompessor
//
//////////////////////////////////////////////////////////////////////////////

namespace RLECompressor {
    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    const WORD RLEMask       = 0xc000;
    const WORD RLEMaskFill   = 0x0000;
    const WORD RLEMaskBYTE   = 0x4000;
    const WORD RLEMaskWORD   = 0x8000;
    const WORD RLEMaskDWORD  = 0xc000;
    const WORD RLELengthMask = 0x3fff;

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    template<class Type>
    void DoStoreSpan(Type** ppd, Stream* pstream, WORD code, WORD count)
    {
        pstream->Write(WORD(code | count));
        pstream->Write((*ppd)[0]);
        (*ppd)            += count;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DoRawStoreSpan(BYTE* pstart, BYTE*& pd, Stream* pstream)
    {
        WORD length = pd - pstart;
        if (length > 0) {
            pstream->Write(WORD(RLEMaskFill | length));
            pstream->Write(pstart, length);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    template<class Type>
    WORD CouldStoreSpan(Type** ppd, Type* pend)
    {
        if ((*ppd) + 1 < pend) {
            Type value = (*ppd)[0];
            WORD count = 0;
    
            while (
                   ((*ppd) + count + 1 < pend) 
                && (*ppd)[count] == value
            ) {
                count++;
            }

            if (count * sizeof(Type) > 2 + sizeof(Type)) {
                return min(count, RLELengthMask);
            }
        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<MemoryStream> Compress(const Span& span)
    {
        TRef<MemoryStream> pstream = new MemoryStream();

        BYTE* pend   = span.m_pbyte + span.m_length;
        BYTE* pstart = span.m_pbyte;
        BYTE* pd     = span.m_pbyte;

        while (pd < pend) {
            WORD count;
            if (count = CouldStoreSpan((DWORD**)&pd, (DWORD*)pend)) {
                DoRawStoreSpan(pstart, pd, pstream);
                DoStoreSpan((DWORD**)&pd, pstream, RLEMaskDWORD, count);
                pstart = pd;
            } else if (count = CouldStoreSpan((WORD**)&pd, (WORD*)pend)) {
                DoRawStoreSpan(pstart, pd, pstream);
                DoStoreSpan((WORD**)&pd, pstream, RLEMaskWORD, count);
                pstart = pd;
            } else if (count = CouldStoreSpan((BYTE**)&pd, (BYTE*)pend)) {
                DoRawStoreSpan(pstart, pd, pstream);
                DoStoreSpan((BYTE**)&pd, pstream, RLEMaskBYTE, count);
                pstart = pd;
            }

            pd++;
        }

        DoRawStoreSpan(pstart, pd, pstream);

        return pstream;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<MemoryStream> Decompress(const Span& span)
    {
        TRef<MemoryStream> pstream = new MemoryStream();

        BYTE* prle = span.m_pbyte;
        BYTE* pend = span.m_pbyte + span.m_length;

        while (prle < pend) {
            WORD word; Extract(prle, word);
            int length = word & RLELengthMask;

            switch (word & RLEMask) {
                case RLEMaskFill:
                    {
                        pstream->Write(prle, length);
                        prle += length;
                    }
                    break;

                case RLEMaskBYTE:
                    {
                        BYTE byte; Extract(prle, byte);

                        pstream->WriteMultiple(byte, length);
                    }
                    break;

                case RLEMaskWORD:
                    {
                        WORD word; Extract(prle, word);

                        pstream->WriteMultiple(word, length);
                    }
                    break;

                case RLEMaskDWORD:
                    {
                        DWORD dword; Extract(prle, dword);

                        pstream->WriteMultiple(dword, length);
                    }
                    break;
            }
        }

        return pstream;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// DictionaryCompressor
//
//////////////////////////////////////////////////////////////////////////////

namespace DifferentialCompressor {
    #if 1
        typedef char LengthType;
        #define LengthMax 128
    #else
        typedef short LengthType;
        #define LengthMax 32768
    #endif

    const int m_lengthLargestMatch   = LengthMax - 1;
    const int m_lengthLargestNoMatch = LengthMax;

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    class Dictionary {
        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        AllocSpan m_span;
        BYTE*     m_pbyteValid;
        BYTE**    m_ppbyte;
        int       m_length;
        float     m_dtime;
        int       m_maxLength;

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

    public:
        Dictionary(
            const Span& spanOld,
            const Span& spanNew,
            int         maxLength
        ) :
            m_span(spanOld.m_length + spanNew.m_length),
            m_maxLength(maxLength)
        {
            Time timeStart = Time::Now();

            //
            // Make a big dictionary that includes both files
            //

            memcpy(m_span.m_pbyte, spanOld.m_pbyte, spanOld.m_length);
            memcpy(m_span.m_pbyte + spanOld.m_length, spanNew.m_pbyte, spanNew.m_length);

            //
            // Only allow use of the parts of the dictionary that the decompressor will 
            // have access to.
            //

            m_pbyteValid = m_span.m_pbyte + spanOld.m_length;

            //
            // Make a sorted list of all the substrings.
            //

            m_ppbyte = new BYTE*[m_span.m_length];

            for (int  index = 0; index < m_span.m_length; index++) {
                m_ppbyte[index] = m_span.m_pbyte + index;
            }

            printf("Dictionary contains %d strings.\n", m_span.m_length);

            m_length = Sort(0, m_span.m_length);

            printf("Dictionary contains %d Unique Strings\n", m_length);

            m_dtime = Time::Now() - timeStart;
        }

        ~Dictionary()
        {
            delete m_ppbyte;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        float GetDTime()
        {
            return m_dtime;
        }

        void IncreaseValidSize(int  length)
        {
            m_pbyteValid += length;
        }
    
        int GetIndex(BYTE* pbyte)
        {
            return pbyte - m_span.m_pbyte;
        }

        int GetLength(BYTE* pbyte)
        {
            return (m_span.m_pbyte + m_span.m_length) - pbyte;
        }

        int GetSortLength(BYTE* pbyte)
        {
            return min(m_maxLength, (m_span.m_pbyte + m_span.m_length) - pbyte);
        }

        int GetValidLength(BYTE* pbyte)
        {
            return max(0, m_pbyteValid - pbyte);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        //  0 strings are the same
        // -1 string is less that string2
        //  1 string is greater than string2
        //
        //////////////////////////////////////////////////////////////////////////////

        int CompareString(
            BYTE* pbyte1, 
            int   length1, 
            BYTE* pbyte2, 
            int   length2, 
            int&  lengthMatch
        ) {
            int lengthMax = min(length1, length2);

            for (lengthMatch = 0; lengthMatch < lengthMax; lengthMatch++) {
                BYTE b1 = pbyte1[lengthMatch];
                BYTE b2 = pbyte2[lengthMatch];

                if (b1 > b2) {
                    return  1;
                } else if (b1 < b2) {
                    return -1;
                }
            }

            if (length1 > lengthMax) {
                return  1;
            } else if (length2 > lengthMax) {
                return -1;
            }

            return 0;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        int Sort(int min, int max)
        {
            if (max - min <= 1) {
                return max;
            }

            if (max - min > 10000) {
                printf("Sorting %d to %d          \r", min, max);
            }

            int   indexMiddle  = (min + max) / 2;
            BYTE* pbyteMiddle  = m_ppbyte[indexMiddle];
            int   lengthMiddle = GetSortLength(pbyteMiddle);
            int   left         = min;
            int   right        = max - 1;
            int   leftWrite    = left;
            int   rightWrite   = right;
    
            while (left <= right) {
                //
                // Find the next left entry that needs to be swapped
                //

                while (left <= right) {
                    int length;
                    int result = 
                        CompareString(
                            m_ppbyte[left],
                            GetSortLength(m_ppbyte[left]),
                            pbyteMiddle,
                            lengthMiddle,
                            length
                        );

                    if (result == 0) {
                        pbyteMiddle = min(pbyteMiddle, m_ppbyte[left]);
                        left++;                        
                    } else if (result == -1) {
                        m_ppbyte[leftWrite] = m_ppbyte[left];
                        leftWrite++;
                        left++;
                    } else {
                        break;
                    }
                }

                //
                // Find the next right entry that needs to be swapped
                //

                while (left <= right) {
                    int length;
                    int result = 
                        CompareString(
                            m_ppbyte[right],
                            GetSortLength(m_ppbyte[right]),
                            pbyteMiddle,
                            lengthMiddle,
                            length
                        );

                    if (result == 0) {
                        pbyteMiddle = min(pbyteMiddle, m_ppbyte[right]);
                        right--;                        
                    } else if (result == 1) {
                        m_ppbyte[rightWrite] = m_ppbyte[right];
                        rightWrite--;
                        right--;
                    } else {
                        break;
                    }
                }

                //
                // Swap them
                //

                if (left < right) {
                    BYTE* pbyte = m_ppbyte[left];
                    m_ppbyte[ leftWrite] = m_ppbyte[right];
                    m_ppbyte[rightWrite] = pbyte;
                    leftWrite++;
                    left++;
                    rightWrite--;
                    right--;
                }
            }

            //
            // Sort the left
            //

            int leftMax = Sort(min, leftWrite);

            //
            // Insert the middle
            //

            m_ppbyte[leftMax] = pbyteMiddle;

            //
            // Move the right
            //

            int rightSize = max - (rightWrite + 1);

            for (int index = 0; index < rightSize; index++) {
                m_ppbyte[leftMax + 1 + index] = m_ppbyte[rightWrite + 1 + index];
            }

            //
            // Sort the right
            //

            return Sort(leftMax + 1, leftMax + 1 + rightSize);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // Find the longest match between the span and the dictionary
        //
        //////////////////////////////////////////////////////////////////////////////

        Span FindLongestMatch(const Span& span)
        {
            int min = 0;
            int max = m_length;
            int indexLongest  = 0;
            int lengthLongest = 0;

            while (min <= max) {
                int index = (min + max) / 2;
                int length;

                int result = 
                    CompareString(
                        span.m_pbyte, 
                        span.m_length, 
                        m_ppbyte[index], 
                        GetLength(m_ppbyte[index]),
                        length
                    );

                if (length > lengthLongest) {
                    lengthLongest = length;
                    indexLongest  = index;
                }

                if (result == 0) {
                    break;
                } else if (result == 1) {
                    min = index + 1;
                } else {
                    if (index == 0) {
                        break;
                    }
                    max = index - 1;
                }
            }

            //
            // Find the longest valid span near the longest span
            //

            BYTE* pbyteLongest = m_ppbyte[indexLongest];

            if (lengthLongest > GetValidLength(pbyteLongest)) {
                //
                // Shorten the length
                //

                lengthLongest = GetValidLength(pbyteLongest);

                //
                // Search forward
                //

                int index = indexLongest + 1;

                while (index < m_length) {
                    int length;

                    int result = 
                        CompareString(
                            span.m_pbyte, 
                            span.m_length, 
                            m_ppbyte[index], 
                            GetValidLength(m_ppbyte[index]),
                            length
                        );

                    if (length == 0) {
                        break;
                    }

                    if (length > lengthLongest) {
                        pbyteLongest  = m_ppbyte[index];
                        lengthLongest = length;
                    }

                    index++;
                }

                //
                // Search backward
                //

                index = indexLongest - 1;

                while (index > 0) {
                    int length;

                    int result = 
                        CompareString(
                            span.m_pbyte, 
                            span.m_length, 
                            m_ppbyte[index], 
                            GetValidLength(m_ppbyte[index]),
                            length
                        );

                    if (length == 0) {
                        break;
                    }

                    if (length > lengthLongest) {
                        pbyteLongest  = m_ppbyte[index];
                        lengthLongest = length;
                    }

                    index--;
                }
            }

            //
            // return it
            //

            return Span(pbyteLongest, lengthLongest);
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    int m_indexNew    ;
    int m_indexNoMatch;
    int m_countMatch  ;
    int m_countNoMatch;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Write out any unmatched bytes
    //
    //////////////////////////////////////////////////////////////////////////////

    static void WriteNoMatch(Stream* pstream, BYTE* pb)
    {
        while (m_indexNoMatch != m_indexNew) {
            int length = min(m_indexNew - m_indexNoMatch, m_lengthLargestNoMatch);

            pstream->Write(LengthType(1 - length));
            pstream->Write(pb + m_indexNoMatch, length);

            m_indexNoMatch += length;
            m_countNoMatch += length;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // File format
    //
    // short length
    // if (length > 0) {
    //     uint index
    // } else {
    //     BYTE data[1 - length]
    // }
    //
    //////////////////////////////////////////////////////////////////////////////

    static TRef<MemoryStream> Compress(
        const Span& spanOld, 
        const Span& spanNew
    ) {
        Dictionary dictionary(spanOld, spanNew, LengthMax - 1);

        TRef<MemoryStream> pstream = new MemoryStream();

        //
        // Write out the length of the resultant file
        //

        pstream->Write(spanNew.m_length);

        //
        // Compress it
        //

        int indexPrint = 0;

        m_indexNew     = 0;
        m_indexNoMatch = 0;
        m_countMatch   = 0;
        m_countNoMatch = 0;

        while (m_indexNew < spanNew.m_length) {
            if (m_indexNew > indexPrint) {
                printf("Compressing %d\r", m_indexNew);
                indexPrint = m_indexNew + 10000;
            }

            Span span = 
                dictionary.FindLongestMatch(
                    Span(
                        spanNew.m_pbyte + m_indexNew, 
                        min(spanNew.m_length - m_indexNew, m_lengthLargestMatch)
                    )
                );

            if (span.m_length > 6) {
                WriteNoMatch(pstream, spanNew.m_pbyte);

                while (span.m_length > 0) {
                    int length = min(span.m_length, m_lengthLargestMatch);

                    pstream->Write(LengthType(length));

                    int index = dictionary.GetIndex(span.m_pbyte);

                    if (index > 32767) {
                        pstream->Write(short(0x8000 | (index >> 8)));
                        pstream->Write(BYTE(index & 0xff));
                    } else {
                        pstream->Write(short(index));
                    }

                    m_indexNew     += length;
                    m_indexNoMatch += length;
                    span.m_pbyte   += length;
                    span.m_length  -= length;
                    m_countMatch   += length;

                    dictionary.IncreaseValidSize(length);
                }
            } else {
                m_indexNew += max(1, span.m_length);
                dictionary.IncreaseValidSize(1);
            }
        }

        //
        // Write out any remaining unmatched bytes
        //

        WriteNoMatch(pstream, spanNew.m_pbyte);

        //
        // Perf
        //

        printf(
            "%d bytes matched, %d didn't match\n"
            "Building dictionary took %g seconds.\n",
            m_countMatch,
            m_countNoMatch,
            dictionary.GetDTime()
        );

        return pstream;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    static TRef<MemoryStream> Decompress(
        const Span& spanOld,
        const Span& spanCompressed
    ) {
        TRef<MemoryStream> pstream = new MemoryStream();

        BYTE* pbyteCompressed    = spanCompressed.m_pbyte;
        BYTE* pbyteCompressedEnd = spanCompressed.m_pbyte + spanCompressed.m_length;
     
        uint lengthNew; Extract(pbyteCompressed, lengthNew);

        BYTE* pbyteDictionary = new BYTE[spanOld.m_length + lengthNew];
        BYTE* pbyteNew        = pbyteDictionary + spanOld.m_length;

        memcpy(pbyteDictionary, spanOld.m_pbyte, spanOld.m_length);

        while (pbyteCompressed < pbyteCompressedEnd) {
            LengthType length; Extract(pbyteCompressed, length);

            if (length > 0) {
                WORD indexTop; Extract(pbyteCompressed, indexTop);
                int index;

                if (indexTop & 0x8000) {
                    BYTE indexBottom; Extract(pbyteCompressed, indexBottom);
                    index = int(((indexTop & 0x7fff) << 8) | indexBottom);
                } else {
                    index = indexTop;
                }

                memcpy(pbyteNew, pbyteDictionary + index, length);
                pbyteNew += length;
            } else {
                memcpy(pbyteNew, pbyteCompressed, 1 - length); 
                pbyteCompressed += 1 - length;
                pbyteNew        += 1 - length;
            }
        }

        ZAssert(pbyteCompressed == pbyteCompressedEnd);

        pstream->Write(pbyteDictionary + spanOld.m_length, lengthNew);

        delete pbyteDictionary;

        return pstream;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Application
//
//////////////////////////////////////////////////////////////////////////////

class Patch : public Win32App {
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    Patch()
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<MemoryStream> DoCompress(
        const Span& spanOld, 
        const Span& spanNew
    ) {
        Time timeStart = Time::Now();

        printf("Original sizes, old: %d, new: %d\n", spanOld.m_length, spanNew.m_length);

        //
        // RLE Compress the two files
        //

        TRef<MemoryStream> pstreamOld = RLECompressor::Compress(spanOld);
        TRef<MemoryStream> pstreamNew = RLECompressor::Compress(spanNew);

        const Span& spanOldRLE = pstreamOld->GetSpan();
        const Span& spanNewRLE = pstreamNew->GetSpan();

        printf("RLE sizes, old: %d, new: %d\n", spanOldRLE.m_length, spanNewRLE.m_length);

        Time timeRLE = Time::Now();

        //
        // Differential compress
        //

        TRef<MemoryStream> pstreamCompressed = 
            DifferentialCompressor::Compress(
                spanOldRLE,
                spanNewRLE
            );

        //
        // Report times
        //

        printf(
            "RLE  took: %g seconds.\n"
            "Diff took: %g seconds.\n", 
            timeRLE - timeStart, 
            Time::Now() - timeRLE
        );

        //
        // return the stream
        //

        return pstreamCompressed;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<MemoryStream> DoDecompress(
        const Span& spanOld, 
        const Span& spanPatch
    ) {
        TRef<MemoryStream> pstreamUnRLEOld = 
            RLECompressor::Compress(spanOld);

        TRef<MemoryStream> pstreamUndiff = 
            DifferentialCompressor::Decompress(
                pstreamUnRLEOld->GetSpan(),
                spanPatch
            );

        TRef<MemoryStream> pstreamUnRLENew = 
            RLECompressor::Decompress(pstreamUndiff->GetSpan());

        return pstreamUnRLENew;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    void DoCompress(
        const ZString& strOld,
        const ZString& strNew,
        const ZString& strPatch
    ) {
        //
        // Open all the files
        //

        TRef<ZFile> pfileOld   = new ZFile(strOld);

        ZAssert(pfileOld->IsValid());

        TRef<ZFile> pfileNew   = new ZFile(strNew);

        ZAssert(pfileNew->IsValid());

        TRef<ZFile> pfilePatch = new ZWriteFile(strPatch);

        ZAssert(pfilePatch->IsValid());

        //
        // Compress
        //

        //Span spanOld((BYTE*)"abcduhfqeoixkcnmvoadsiufyasdnbxjcvhagd", 20);
        //Span spanNew((BYTE*)"cadbasdfasdfuihqweofiunxvoasdijnvaosid", 20);
        Span spanOld(pfileOld->GetPointer(), pfileOld->GetLength());
        Span spanNew(pfileNew->GetPointer(), pfileNew->GetLength());

        TRef<MemoryStream> pstreamCompressed = DoCompress(spanOld, spanNew);

        //
        // Make sure that decompressing produces the new file.
        //

        TRef<MemoryStream> pstreamDecompressed =
            DoDecompress(
                spanOld,
                pstreamCompressed->GetSpan()
            );

        BYTE* pbyte1 = spanNew.m_pbyte;
        BYTE* pbyte2 = pstreamDecompressed->GetSpan().m_pbyte;
        int   length = spanNew.m_length;

        ZAssert(length == pstreamDecompressed->GetSpan().m_length);

        for(int index = 0; index < length; index++) {
            if (pbyte1[index] != pbyte2[index]) {
                ZError("Decompressed file doesn't match new file.");
            }
        }

        //
        // Write out the file
        //

        pfilePatch->Write(
            pstreamCompressed->GetSpan().m_pbyte,
            pstreamCompressed->GetSpan().m_length
        );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    void DoDecompress(
        const ZString& strOld,
        const ZString& strPatch,
        const ZString& strNew
    ) {
        //
        // Open all the files
        //

        TRef<ZFile> pfileOld   = new ZFile(strOld);
        TRef<ZFile> pfilePatch = new ZFile(strPatch);
        TRef<ZFile> pfileNew   = new ZWriteFile(strNew);

        //
        // Decompress
        //

        TRef<MemoryStream> pstreamDecompressed =
            DoDecompress(
                Span(pfileOld->GetPointer(),   pfileOld->GetLength()),
                Span(pfilePatch->GetPointer(), pfilePatch->GetLength())
            );

        //
        // Write out the file
        //

        pfileNew->Write(
            pstreamDecompressed->GetSpan().m_pbyte,
            pstreamDecompressed->GetSpan().m_length
        );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    HRESULT Initialize(const ZString& strCommandLine)
    {
        PCC pcc = strCommandLine;
        CommandLineToken token(pcc, strCommandLine.GetLength());

        while (token.MoreTokens()) {
            ZString str;
            ZString strOld;
            ZString strNew;
            ZString strPatch;

            if (token.IsMinus(str)) {
                if (str == "compress") {
                    if (token.IsString(strOld)) {
                        if (token.IsString(strNew)) {
                            if (token.IsString(strPatch)) {
                                DoCompress(strOld, strNew, strPatch);
                                return S_FALSE;
                            }
                        }
                    }
                }
            } else if (token.IsString(strOld)) {
                if (token.IsString(strPatch)) {
                    if (token.IsString(strNew)) {
                        DoDecompress(strOld, strPatch, strNew);
                        return S_FALSE;
                    }
                }
            }
            break;
        }

        printf(
            "Usage: patch -compress oldfile newfile patchfile\n"
            "   or  patch oldfile patchfile newfile\n"
        );

        return S_FALSE;
    }
} g_app;
