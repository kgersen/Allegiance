#include "pch.h"
#include "7zAlloc.h"
#include "7zFile.h"
#include "LzmaEnc.h"

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

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
			File_Close(&outStream.file);
			File_Close(&inStream.file);
		}
	}
	return res;
}

char * NextDump() {
	WIN32_FIND_DATA finddata;
	HANDLE hsearchFiles = 0;
    
	char szPathName[MAX_PATH+48] = ""; 
	char szName[MAX_PATH+48] = "";
	char szDest[MAX_PATH+54] = "";
	FILETIME ftTime = {0,0};
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	strcat(szPathName, "*.dmp*");
	hsearchFiles = FindFirstFile(szPathName, &finddata);
	if (INVALID_HANDLE_VALUE == hsearchFiles)
		return NULL;

	while (INVALID_HANDLE_VALUE != hsearchFiles) {
		if (finddata.cFileName[0] != '.') {
			if (CompareFileTime(&finddata.ftCreationTime, &ftTime) == 1)
				strcpy(szName, finddata.cFileName);
		}
		if (!FindNextFile(hsearchFiles, &finddata))
		{
			FindClose(hsearchFiles);
			hsearchFiles = INVALID_HANDLE_VALUE;
		}
	}
	strcat(szDest,".sent");
	MoveFile(szName, szDest);
	return szName;
}

void DeleteDumps(bool bAll) {
	//NYI, delete stale dump files
	//keep the last 3
}