#include "pch.h"
#include "7zAlloc.h"
#include "7zFile.h"
#include "LzmaEnc.h"
#include "LzmaDec.h"

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

ZString GetAppDir() {
	char szPathName[MAX_PATH+48] = "";
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	char*   p = strrchr(szPathName, '\\');
	p = (!p) ? szPathName : p+1;
	strcpy(p,"");
	return ZString(szPathName);
}

static SRes Decode2(CLzmaDec *state, ISeqOutStream *outStream, ISeqInStream *inStream,
    UInt64 unpackSize)
{
  int thereIsSize = (unpackSize != (UInt64)(Int64)-1);
  Byte inBuf[IN_BUF_SIZE];
  Byte outBuf[OUT_BUF_SIZE];
  size_t inPos = 0, inSize = 0, outPos = 0;
  LzmaDec_Init(state);
  for (;;)
  {
    if (inPos == inSize)
    {
      inSize = IN_BUF_SIZE;
      RINOK(inStream->Read(inStream, inBuf, &inSize));
      inPos = 0;
    }
    {
      SRes res;
      SizeT inProcessed = inSize - inPos;
      SizeT outProcessed = OUT_BUF_SIZE - outPos;
      ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
      ELzmaStatus status;
      if (thereIsSize && outProcessed > unpackSize)
      {
        outProcessed = (SizeT)unpackSize;
        finishMode = LZMA_FINISH_END;
      }
      
      res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
        inBuf + inPos, &inProcessed, finishMode, &status);
      inPos += inProcessed;
      outPos += outProcessed;
      unpackSize -= outProcessed;
      
      if (outStream)
        if (outStream->Write(outStream, outBuf, outPos) != outPos)
          return SZ_ERROR_WRITE;
        
      outPos = 0;
      
      if (res != SZ_OK || thereIsSize && unpackSize == 0)
        return res;
      
      if (inProcessed == 0 && outProcessed == 0)
      {
        if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
          return SZ_ERROR_DATA;
        return res;
      }
    }
  }
}

static SRes Decode(ISeqOutStream *outStream, ISeqInStream *inStream)
{
  UInt64 unpackSize;
  int i;
  SRes res = 0;

  CLzmaDec state;

  /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
  unsigned char header[LZMA_PROPS_SIZE + 8];

  /* Read and parse header */

  RINOK(SeqInStream_Read(inStream, header, sizeof(header)));

  unpackSize = 0;
  for (i = 0; i < 8; i++)
    unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);

  LzmaDec_Construct(&state);
  RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc));
  res = Decode2(&state, outStream, inStream, unpackSize);
  LzmaDec_Free(&state, &g_Alloc);
  return res;
}

static SRes Encode(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize, char *rs)
{
  CLzmaEncHandle enc;
  SRes res;
  CLzmaEncProps props;
  rs = rs;

  enc = LzmaEnc_Create(&g_Alloc);
  if (enc == 0)
    return SZ_ERROR_MEM;

  LzmaEncProps_Init(&props);
  res = LzmaEnc_SetProps(enc, &props);

  if (res == SZ_OK)
  {
    Byte header[LZMA_PROPS_SIZE + 8];
    size_t headerSize = LZMA_PROPS_SIZE;
    int i;

    res = LzmaEnc_WriteProperties(enc, header, &headerSize);
    for (i = 0; i < 8; i++)
      header[headerSize++] = (Byte)(fileSize >> (8 * i));
    if (outStream->Write(outStream, header, headerSize) != headerSize)
      res = SZ_ERROR_WRITE;
    else
    {
      if (res == SZ_OK)
        res = LzmaEnc_Encode(enc, outStream, inStream, NULL, &g_Alloc, &g_Alloc);
    }
  }
  LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);
  return res;
}

int Create7z(const char * szFile, const char * sz7z) {
	CFileSeqInStream inStream;
	CFileOutStream outStream;
	int res = -1;
	int res2 = -1;
	UInt64 iLength = 0;
	Bool useOutFile = False;
	FileSeqInStream_CreateVTable(&inStream);
	File_Construct(&inStream.file);
	FileOutStream_CreateVTable(&outStream);
	File_Construct(&outStream.file);

	if (InFile_Open(&inStream.file, szFile) == 0) {
		if (OutFile_Open(&outStream.file, sz7z) == 0) {
			UInt64 fileSize;
			File_GetLength(&inStream.file, &fileSize);
			char * rs = NULL;
			res = Encode(&outStream.s, &inStream.s, fileSize, rs);
			res2 = File_GetLength(&outStream.file,&iLength);
			File_Close(&outStream.file);
			File_Close(&inStream.file);
		}
	}
	
	return (res != 0 || res2 != 0 || iLength == 0) ? -1 : iLength;
}

FileList FindDumps() {
	FileList tlFiles;
	tlFiles.SetEmpty();

	WIN32_FIND_DATA finddata;
	ZeroMemory(&finddata,sizeof(WIN32_FIND_DATA));

	char szPathName[MAX_PATH+48] = "";
	char szName[MAX_PATH+48] = "";	
	strcpy(szPathName,(PCC)GetAppDir());
	strcpy(szName,szPathName);
    strcat(szPathName, "*.dmp");
	
	HANDLE hsearchFiles = 0;
	FILETIME ftTime = {0,0};
	hsearchFiles = FindFirstFile(szPathName, &finddata);

	if (INVALID_HANDLE_VALUE == hsearchFiles)
		return tlFiles;

	int iKBytes = 0;
	while (INVALID_HANDLE_VALUE != hsearchFiles) {
		if (finddata.cFileName[0] != '.') {
			char*   p = strrchr(szName, '\\');
			p = (!p) ? szName : p+1;
			strcpy(p, "");
			strcat(szName, finddata.cFileName);
			tlFiles.InsertSorted(finddata);
			iKBytes += (finddata.nFileSizeHigh > 0) ? MAXINT : (finddata.nFileSizeLow / 1024);
			if (iKBytes >= MAXINT)
				return tlFiles; 
		}
		if (!FindNextFile(hsearchFiles, &finddata)){
			FindClose(hsearchFiles);
			hsearchFiles = INVALID_HANDLE_VALUE;
		}
	}
	return tlFiles;
}

void DeleteDumps(bool bDelete) {
	FileList tlFiles = FindDumps();
	char szDir[MAX_PATH+52] = "";
	strcpy(szDir,(PCC)GetAppDir());
	if (!tlFiles.IsEmpty()) {
		for (FileList::Iterator iterFile(tlFiles);
			!iterFile.End(); iterFile.Next())
		{
			ZString zFile = ZString(szDir)+ZString(iterFile.Value().cFileName);
			if (!bDelete) {
				MoveFile((PCC)zFile,zFile+ZString(".old"));
				continue;
			}
			else {
				debugf("**** Deleting %s\n",(PCC)zFile);
				DeleteFile(zFile);
			}
		}
	}
}

