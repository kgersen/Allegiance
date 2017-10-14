#pragma once

class CLogFile
{
private:
	HANDLE m_hLogFile;
	char m_szFilePath[ MAX_PATH ];
	char m_szFileName[ MAX_PATH ];
	char m_szTimeStamp[256];

public:
	CLogFile( char * szLogName );
	~CLogFile(void);

	void CloseLogFile();
	void OutputString( const char * szString );
	void OutputStringV( const char * szFormat, ... );
	char * GetTimeStampString();
};
