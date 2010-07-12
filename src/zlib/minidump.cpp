#include "pch.h"
#include "7zAlloc.h"
#include "7zFile.h"
#include "LzmaEnc.h"

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };


ZString GetAppDir() {
	char szPathName[MAX_PATH+48] = "";
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	char*   p = strrchr(szPathName, '\\');
	p = (!p) ? szPathName : p+1;
	strcpy(p,"");
	return ZString(szPathName);
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

void DeleteDumps(bool bAll) {
	FileList tlFiles = FindDumps();
	int iCount = 1;
	if (!tlFiles.IsEmpty()) {
		for (FileList::Iterator iterFile(tlFiles);
			!iterFile.End(); iterFile.Next())
		{
			if (bAll == false && iCount <= 3) {
				iCount++;
				continue;
			}
			else
				DeleteFile((PCC)ZString(iterFile.Value().cFileName));

			iCount++;
		}
	}
}

