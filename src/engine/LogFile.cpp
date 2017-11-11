#include "LogFile.h"

#include <cstdio>
#include <ctime>
#include <zassert.h>

////////////////////////////////////////////////////////////////////////////////
// CLogFile()
// Constructor.
////////////////////////////////////////////////////////////////////////////////
CLogFile::CLogFile( const char * szFileName )
{
	m_hLogFile = INVALID_HANDLE_VALUE;

	strcpy_s( m_szFileName, MAX_PATH, szFileName );
	GetModuleFileName( NULL, m_szFilePath, MAX_PATH);

	char * p = strrchr(m_szFilePath, '\\');
	if (!p)
		p = m_szFilePath;
	else
		p++;

	strcpy_s( p, MAX_PATH - ( strlen( m_szFilePath ) - strlen( p ) ), szFileName );

	m_hLogFile = CreateFile(	m_szFilePath, 
								GENERIC_WRITE, 
								0,
								NULL, 
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
								NULL );
	ZAssert( m_hLogFile != INVALID_HANDLE_VALUE );

	OutputStringV( "Log file %s created - %s\n\n", szFileName, GetTimeStampString() );
}


////////////////////////////////////////////////////////////////////////////////
// ~CLogFile()
// Destructor.
////////////////////////////////////////////////////////////////////////////////
CLogFile::~CLogFile(void)
{
	CloseLogFile();
}


////////////////////////////////////////////////////////////////////////////////
// CloseLogFile()
//
////////////////////////////////////////////////////////////////////////////////
void CLogFile::CloseLogFile()
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		OutputString( "\nLog closed.\n" );
		CloseHandle( m_hLogFile );
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
}


////////////////////////////////////////////////////////////////////////////////
// OutputString()
// Output a standard string to the log file.
////////////////////////////////////////////////////////////////////////////////
void CLogFile::OutputString( const char * szString )
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		DWORD nBytes;
		::WriteFile(m_hLogFile, szString, strlen(szString), &nBytes, NULL);
	}
	debugf(szString);
}


////////////////////////////////////////////////////////////////////////////////
// LogString()
//
////////////////////////////////////////////////////////////////////////////////
void CLogFile::OutputStringV( const char * szFormat, ... )
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		const size_t size = 2048;			// Based on debugf value.
		char szTemp[ size ];
		va_list vl;
		va_start( vl, szFormat );
		_vsnprintf_s( szTemp, size, (size-1), szFormat, vl );
		va_end(vl);
		OutputString( szTemp );
	}
}


////////////////////////////////////////////////////////////////////////////////
// GetTimeStampString()
// Returns the output buffer as a string for use as a input parameter.
////////////////////////////////////////////////////////////////////////////////
char * CLogFile::GetTimeStampString()
{
	time_t longTime;
	tm t;

	time(&longTime);
	localtime_s( &t, &longTime);

	const char * months[] = {"Jan", "Feb", "Mar", "Apr",
							"May", "Jun", "Jul", "Aug",
							"Sep", "Oct", "Nov", "Dec"};
	strcpy_s( m_szTimeStamp, 256, months[ t.tm_mon ] );
	sprintf_s( m_szTimeStamp+3, 256 - 3, " %02d, %02d:%02d", t.tm_mday, t.tm_hour, t.tm_min );

	return m_szTimeStamp;
}
