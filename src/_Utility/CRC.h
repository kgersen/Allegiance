#pragma once

#include <windows.h>

/***************************************************************************
Function:		MemoryCRC()
Parameters:		data: pointer to data
				size: size of the data (must be non-zero)
Purpose:		generate a 32-bit checksum
Returns:		checksum of the data (never returns zero)
****************************************************************************/
int MemoryCRC(const void *_data, unsigned size);


/***************************************************************************
Function:		FileCRC()
Parameters:		szFileName:   file to get CRC from
                szErrorMsg:   this can be NULL if you don't care about a msg,
                              or else it must be able to store 100+MAX_PATH chars
Returns:        Checksum of the file, returns 0 on error
Example:		CRC = FileCRC("autoexec.bat", 0, 0);
****************************************************************************/
int FileCRC(const char *szFilename, char * szErrorMsg);
int FileCRC(HANDLE hFile,           char * szErrorMsg);
