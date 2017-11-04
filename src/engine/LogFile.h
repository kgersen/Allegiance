#pragma once

#include <windows.h>

class CLogFile
{
private:
	HANDLE m_hLogFile;
	char m_szFilePath[ MAX_PATH ];
	char m_szFileName[ MAX_PATH ];
	char m_szTimeStamp[256];

public:
    CLogFile(const char *szLogName );
	~CLogFile(void);

	void CloseLogFile();
	void OutputString( const char * szString );
	void OutputStringV( const char * szFormat, ... );
	char * GetTimeStampString();
};
