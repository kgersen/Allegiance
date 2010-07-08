#include "pch.h"
#include "7zAlloc.h"
#include "7zFile.h"
#include "LzmaEnc.h"

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };


char * GetAppDir() {
	char szPathName[MAX_PATH+48] = "";
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	char*   p = strrchr(szPathName, '\\');
	p = (!p) ? szPathName : p+1;
	strcpy(p,"");
	return szPathName;
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

FileList FindDumps() {
	FileList tlFiles;
	tlFiles.SetEmpty();

	WIN32_FIND_DATA finddata;
	ZeroMemory(&finddata,sizeof(WIN32_FIND_DATA));

	char szPathName[MAX_PATH+48] = "";
	char szName[MAX_PATH+48] = "";	
	strcpy(szPathName,GetAppDir());
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

void DeleteDumps(bool bAll) {
	//NYI, delete stale dump files
	//keep the last 3
}

