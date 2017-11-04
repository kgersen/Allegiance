#include "DX9PackFile.h"

#include <base.h>
#include <zassert.h>

#include "EngineSettings.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
CDX9PackFile *	CDX9PackFile::m_pPackFileLinkedList = NULL;


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
CPackExclusion::CPackExclusion()
{
	m_pNext = NULL;
	m_eType = eET_Undefined;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
CPackExclusion::CPackExclusion( const char * szFilter )
{
	// Determine filter type from string.
	ZString szTemp = szFilter;
	szTemp = szTemp.ToLower();
	if( szTemp.Find( "*" ) != -1 )
	{
		// Substring filter.
		m_eType = eET_SubString;

		if( szTemp.Left(1) == "*" )
		{
			m_szExclusionFilter = &szTemp[1];
		}
		else if( szTemp.Right(1) == "*" )
		{
			m_szExclusionFilter = szTemp.Left( szTemp.GetLength() - 1 );
		}
		else
		{
			m_eType = eET_Undefined;
            ZAssert( false && "Not supported, feel free to add..." );
		}
	}
	else
	{
		// Filename match.
		m_eType = eET_FileName;
		m_szExclusionFilter = szFilter;
	}
	m_pNext = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// IsExcluded()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPackExclusion::IsExcluded( const char * szFileName )
{
	ZString test = szFileName;
	test = test.ToLower();

	switch( m_eType )
	{
	case eET_FileName:
		if( m_szExclusionFilter == test )
		{
			return true;
		}
		break;

	case eET_SubString:
		if( test.Find( m_szExclusionFilter ) != -1 )
		{
			return true;
		}
		break;

	default:
        ZAssert( false && "Invalid type for exclusion." );
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
CDX9PackFile::CDX9PackFile( const char * szDataPath, const char * szFileName )
{
	m_pNext				= NULL;
	m_pFile				= NULL;
	m_pHashTable		= NULL;
	m_pWriteBuffer		= NULL;
	m_pExclusionList	= NULL;
	m_fnCallBack		= NULL;
	m_pPackFile			= NULL;
	m_pHashEntryArray	= NULL;
	m_pRawFileData		= NULL;

	m_bUserCancelled	= false;
	m_bFinished			= false;
	m_iTotalFiles		= 0;
	m_iCurrentFile		= 0;
	m_dwPackOffset		= 0;
	m_dwWritePos		= 0;
	m_szDataPath		= szDataPath;
	m_szFileName		= szFileName;
	m_szFileName		+= ".pack";
	m_szOutputFileName	= m_szDataPath + "/" + m_szFileName;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
CDX9PackFile::~CDX9PackFile(void)
{
	if( m_pFile )
	{
		delete m_pFile;
		m_pFile = NULL;
	}
	if( m_pPackFile )
	{
		delete m_pPackFile;
		m_pPackFile = NULL;
	}
	m_pHashEntryArray	= NULL;
	m_pRawFileData		= NULL;
	m_pNext				= NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Exists()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDX9PackFile::Exists()
{
	bool bRetVal;
	
	m_pFile	= new ZFile( m_szOutputFileName );
	bRetVal = m_pFile->IsValid();
	delete m_pFile;
	m_pFile = NULL;
	return bRetVal;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Create()
// TODO: make this extensible so that files can be included or excluded from the search properly.
// As this is just a test, for now we assume we add bmp.mdl files and .png files only, and those
// in the root directory or the textures directory.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDX9PackFile::Create( PACK_CREATE_CALLBACK pFnCreateCallback )
{
	long headerWritePos;

	m_fnCallBack = pFnCreateCallback;

	// Create our write buffer.
	m_pWriteBuffer = new BYTE[ s_iWriteBufferSize ];

	// Create the file for writing.
	m_pFile = new ZFile( m_szOutputFileName, OF_WRITE | OF_SHARE_DENY_WRITE | OF_CREATE );
	headerWritePos = m_pFile->Tell();
	m_dwWritePos	= 0;
	m_dwPackOffset	= 0;
	memset( &m_header, 0, sizeof( SPackFileHeader ) );

	// Create header.
	SPackFileHeader placeholderHeader;
	m_header.dwFileDataOffset = sizeof( SPackFileHeader );
	m_header.dwTableOffset = 0xFFFFFFFF;
	memcpy( m_header.szID, "AllegPak", 8 );
	memset( &placeholderHeader, 0xFF, sizeof( SPackFileHeader ) );

	// Write out a placeholder for the header.
	m_pFile->Write( &placeholderHeader, sizeof( SPackFileHeader ) );

	// Add filters for stopping certain files being added.
	AddExclusionFilter( "bsg_*" );
	AddExclusionFilter( "hlp*" );
	AddExclusionFilter( "sw_*" );
	AddExclusionFilter( "tm_*" );
	AddExclusionFilter( "vos_*" );
	AddExclusionFilter( "train*" );
	AddExclusionFilter( "zone*" );
	AddExclusionFilter( "lobby*" );
	AddExclusionFilter( "dialog*" );
	AddExclusionFilter( "header*" );
	AddExclusionFilter( "loadout*" );
	AddExclusionFilter( "screen*" );
	AddExclusionFilter( "turretHUDbmp.mdl" );

	// Create the new hash table for our data.
	m_pHashTable = new CHashTable();

	// Calculate the total number of files.
	m_iTotalFiles  = GetFileCount( "Textures", "*bmp.mdl" );
	m_iTotalFiles += GetFileCount( "Textures", "*.png" );
	m_iTotalFiles += GetFileCount( "", "*bmp.mdl" );
	m_iTotalFiles += GetFileCount( "", "*.png" );

	delete m_pHashTable;
	m_pHashTable = new CHashTable();

	// Add files passed in by folder relative to root path and a 
	// search filter.
	m_iCurrentFile = 0;
	AddFiles( "Textures", "*bmp.mdl" );
	if( m_bUserCancelled == true )
	{
		delete m_pFile;
		m_pFile = NULL;
		delete m_pHashTable;
		m_pHashTable = NULL;
		m_bFinished = true;
		DeleteFile( &m_szOutputFileName[0] );
		return false;
	}
	AddFiles( "Textures", "*.png" );
	if( m_bUserCancelled == true )
	{
		delete m_pFile;
		m_pFile = NULL;
		delete m_pHashTable;
		m_pHashTable = NULL;
		m_bFinished = true;
		DeleteFile( &m_szOutputFileName[0] );
		return false;
	}
	AddFiles( "", "*bmp.mdl" );
	if( m_bUserCancelled == true )
	{
		delete m_pFile;
		m_pFile = NULL;
		delete m_pHashTable;
		m_pHashTable = NULL;
		m_bFinished = true;
		DeleteFile( &m_szOutputFileName[0] );
		return false;
	}
	AddFiles( "", "*.png" );
	if( m_bUserCancelled == true )
	{
		delete m_pFile;
		m_pFile = NULL;
		delete m_pHashTable;
		m_pHashTable = NULL;
		m_bFinished = true;
		DeleteFile( &m_szOutputFileName[0] );
		return false;
	}

	// Any data left over?
	if( m_dwWritePos != 0 )
	{
		// No, flush the existing file.
		DWORD dwAmountWritten = m_pFile->Write( m_pWriteBuffer, m_dwWritePos );
        ZAssert( dwAmountWritten == m_dwWritePos );
		m_dwWritePos = 0;
	}

	m_header.dwFileDataSize = m_dwPackOffset;
	m_header.dwTableOffset = m_header.dwFileDataSize + m_header.dwFileDataOffset;

	// Dump hash table data.
	WriteHashTableData( );

	// Finally, update the header and write out.
	m_pFile->Seek( headerWritePos, 0 );
	m_pFile->Write( &m_header, sizeof( SPackFileHeader ) );

	delete m_pWriteBuffer;
	delete m_pHashTable;
	delete m_pFile;
	m_pWriteBuffer = NULL;
	m_pHashTable = NULL;
	m_pFile = NULL;

	// Signal we're finished.
	m_bFinished = true;
	m_fnCallBack( -1, -1 );
	return true;
}

#define PACK_WRITE_BUFFER_SIZE ( 64 * 1024 * 1024 )

////////////////////////////////////////////////////////////////////////////////
// AddFiles()
//
////////////////////////////////////////////////////////////////////////////////
bool CDX9PackFile::AddFiles( ZString szDir, ZString szFilter )
{
	DWORD dwRet;
	BOOL bFoundFile = TRUE, bAddFile;
	TCHAR szOriginalDir[ MAX_PATH ];//, szSearchDir[ MAX_PATH ];
	ZString szSearchDir, szFile;
	HANDLE hCurrentFile;
	WIN32_FIND_DATA findData;
	DWORD dwAmountWritten, dwAmountRead, dwHash;
	CPackFileHashEntry * pTableEntry;
	CHashTable tempTable;
	bool bRetVal = true;

	dwRet = GetCurrentDirectory( MAX_PATH, szOriginalDir );
    ZAssert( ( dwRet > 0 ) && ( dwRet < MAX_PATH ) );

	szSearchDir = m_szDataPath;
	if( szDir != "" )
	{
		szSearchDir += "/" + szDir;
	}

	SetCurrentDirectory( szSearchDir );

	hCurrentFile = FindFirstFile( szFilter, &findData );
	if( hCurrentFile == INVALID_HANDLE_VALUE )
	{
		bFoundFile = FALSE;
	}
	while( bFoundFile == TRUE )
	{
		if( m_bUserCancelled == true )
		{
			bRetVal = false;
			break;
		}

		if( ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 ) &&
			(	( findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ) ||
				( findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) ||
				( findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ) ) )
		{
			CPackExclusion * pExclusion = m_pExclusionList;

			// Check file name against the exclusion list.
			while( pExclusion != NULL )
			{
				if( pExclusion->IsExcluded( findData.cFileName ) == true )
				{
					break;
				}
				pExclusion = pExclusion->m_pNext;
			}

			if( pExclusion == NULL )
			{
				CPackFileHashEntry hashEntry;

				bAddFile = TRUE;		// Default is to add.

				// Can't handle files that are > 4gig... gulp.
                ZAssert( findData.nFileSizeHigh == 0 );

				szFile						= findData.cFileName;
				hashEntry.mszFileName		= szFile.ToLower();
				hashEntry.m_dwPackOffset	= m_dwPackOffset;
				hashEntry.m_dwFileSize		= findData.nFileSizeLow;

				dwHash = CHashTable::GenerateHashValue( &hashEntry.mszFileName[0] );
				pTableEntry = (CPackFileHashEntry*) m_pHashTable->GetHashEntry( dwHash );
				if( pTableEntry != NULL )
				{
					// Check that the names match. If they don't it's a genuine hash clash.
                    ZAssert( pTableEntry->mszFileName == hashEntry.mszFileName );
					bAddFile = FALSE;
				}
				else
				{
					m_iCurrentFile ++;
					m_fnCallBack( m_iCurrentFile, m_iTotalFiles );

					pTableEntry = new CPackFileHashEntry();
					
					pTableEntry->SetHashValue( dwHash );
					pTableEntry->m_dwPackOffset = hashEntry.m_dwPackOffset;
					pTableEntry->m_dwFileSize	= hashEntry.m_dwFileSize;
					pTableEntry->mszFileName	= hashEntry.mszFileName;

					// Is there room to add the new file?
					if( m_dwWritePos + findData.nFileSizeLow > PACK_WRITE_BUFFER_SIZE )
					{
						// No, flush the existing file.
						dwAmountWritten = m_pFile->Write( m_pWriteBuffer, m_dwWritePos );
                        ZAssert( dwAmountWritten == m_dwWritePos );

						m_dwWritePos = 0;
					}

					// Doesn't already exist, add now. Import the file.
					ZFile addedFile( szFile );
					dwAmountRead = addedFile.Read( &m_pWriteBuffer[ m_dwWritePos ], findData.nFileSizeLow );
                    ZAssert( dwAmountRead == findData.nFileSizeLow );

					// Update the write position and pack pointer.
					m_dwWritePos	+= dwAmountRead;
					m_dwPackOffset	+= dwAmountRead;

					// 4 byte align.
					if( m_dwWritePos % 4 != 0 )
					{
						m_dwWritePos += 4 - ( m_dwWritePos % 4 );
					}
					if( m_dwPackOffset % 4 != 0 )
					{
						m_dwPackOffset += 4 - ( m_dwPackOffset % 4 );
					}
                    ZAssert( m_dwWritePos % 4 == 0 );
                    ZAssert( m_dwPackOffset % 4 == 0 );

					// Add the to the hash table.
					m_pHashTable->AddHashEntry( pTableEntry );
				}
			}
		}

		bFoundFile = FindNextFile( hCurrentFile, &findData );
	}
	FindClose( hCurrentFile );
	
	// Restore the original directory.
	SetCurrentDirectory( szOriginalDir );
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// AddExclusionFilter()
//
////////////////////////////////////////////////////////////////////////////////
void CDX9PackFile::AddExclusionFilter( const char * szFilter )
{
	CPackExclusion * pExclusion = new CPackExclusion( szFilter );

	if( pExclusion->m_eType == CPackExclusion::eET_Undefined )
	{
		delete pExclusion;
	}
	else
	{
		// Add to END of list.
		CPackExclusion * pList = m_pExclusionList;

		if( pList == NULL )
		{
			m_pExclusionList = pExclusion;
		}
		else
		{
			while( pList->m_pNext != NULL )
			{
				pList = pList->m_pNext;
			}
			pList->m_pNext = pExclusion;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// AddExclusionFilter()
//
////////////////////////////////////////////////////////////////////////////////
int CDX9PackFile::GetFileCount( ZString szDir, ZString szFilter )
{
	DWORD dwRet;
	BOOL bFoundFile = TRUE, bAddFile;
	TCHAR szOriginalDir[ MAX_PATH ];//, szSearchDir[ MAX_PATH ];
	ZString szSearchDir, szFile;
	HANDLE hCurrentFile;
	WIN32_FIND_DATA findData;
	DWORD dwHash;
	CPackFileHashEntry * pTableEntry;

	dwRet = GetCurrentDirectory( MAX_PATH, szOriginalDir );
    ZAssert( ( dwRet > 0 ) && ( dwRet < MAX_PATH ) );

	szSearchDir = m_szDataPath;
	if( szDir != "" )
	{
		szSearchDir += "/" + szDir;
	}

	SetCurrentDirectory( szSearchDir );

	int iMaxFiles = 0;
	hCurrentFile = FindFirstFile( szFilter, &findData );
	if( hCurrentFile == INVALID_HANDLE_VALUE )
	{
		bFoundFile = FALSE;
	}
	while( bFoundFile == TRUE )
	{
		if( ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 ) &&
			(	( findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ) ||
				( findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) ||
				( findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ) ) )
		{
			CPackExclusion * pExclusion = m_pExclusionList;

			// Check file name against the exclusion list.
			while( pExclusion != NULL )
			{
				if( pExclusion->IsExcluded( findData.cFileName ) == true )
				{
					break;
				}
				pExclusion = pExclusion->m_pNext;
			}

			if( pExclusion == NULL )
			{
				CPackFileHashEntry hashEntry;

				bAddFile = TRUE;		// Default is to add.

				// Can't handle files that are > 4gig... gulp.
                ZAssert( findData.nFileSizeHigh == 0 );

				szFile					= findData.cFileName;
				hashEntry.mszFileName	= szFile.ToLower();

				dwHash = CHashTable::GenerateHashValue( &hashEntry.mszFileName[0] );
				pTableEntry = (CPackFileHashEntry*) m_pHashTable->GetHashEntry( dwHash );
				if( pTableEntry == NULL )
				{
					pTableEntry = new CPackFileHashEntry();
					pTableEntry->SetHashValue( dwHash );
					pTableEntry->mszFileName = hashEntry.mszFileName;

					// Add the to the hash table.
					m_pHashTable->AddHashEntry( pTableEntry );
					iMaxFiles ++;
				}
			}
		}
		bFoundFile = FindNextFile( hCurrentFile, &findData );
	}
	FindClose( hCurrentFile );

	// Restore the original directory.
	SetCurrentDirectory( szOriginalDir );

	return iMaxFiles;
}


////////////////////////////////////////////////////////////////////////////////
// WriteHashTableData()
//
////////////////////////////////////////////////////////////////////////////////
bool CDX9PackFile::WriteHashTableData( )
{
	CPackFileHashEntry * pEntry;
	SHashTableEntry * pHashData;
	DWORD dwAddIndex = m_pHashTable->GetTableSize(), dwInitialAdd, dwNumChildren;
	DWORD dwTableSize = 0, dwAmountWritten;

	m_header.dwTableSize = m_pHashTable->GetTableSize();
	m_header.dwTableEntrySize = sizeof( SHashTableEntry );

	// Calculate table size first.
	for( DWORD i=0; i<m_pHashTable->GetTableSize(); i++ )
	{
		pEntry = (CPackFileHashEntry*) m_pHashTable->GetLeadHashEntryByIndex( i );
		dwNumChildren = m_pHashTable->GetNumChildren( pEntry );
		dwTableSize += dwNumChildren + 1;		// +1 for parent.
	}

	pHashData = new SHashTableEntry[ dwTableSize ];
	m_header.dwNumTableEntries = dwTableSize;
	m_header.dwTableDataSize = dwTableSize * sizeof( SHashTableEntry ); 

	// Populate the table.
	for( DWORD i=0; i<m_pHashTable->GetTableSize(); i++ )
	{
		pEntry = (CPackFileHashEntry*) m_pHashTable->GetLeadHashEntryByIndex( i );

		if( pEntry == NULL )
		{
			memset( &pHashData[i], 0, sizeof( SHashTableEntry ) );
		}
		else
		{
			dwNumChildren = m_pHashTable->GetNumChildren( pEntry );
			dwInitialAdd = dwAddIndex;

			pHashData[i].dwDataOffset = pEntry->m_dwPackOffset;
			pHashData[i].dwHashValue = pEntry->GetHashValue();
			pHashData[i].dwFileSize = pEntry->m_dwFileSize;

			if( pEntry->GetNext() != NULL )
			{
				pHashData[i].dwNextIndex = dwAddIndex;
				pEntry = (CPackFileHashEntry*) pEntry->GetNext();
				while( pEntry != NULL )
				{
					pHashData[ dwAddIndex ].dwDataOffset = pEntry->m_dwPackOffset;
					pHashData[ dwAddIndex ].dwHashValue = pEntry->GetHashValue();
					pHashData[ dwAddIndex ].dwNextIndex = pEntry->GetNext() == NULL ? 0 : dwAddIndex + 1;
					pHashData[ dwAddIndex ].dwFileSize = pEntry->m_dwFileSize;
					dwAddIndex ++;
					pEntry = (CPackFileHashEntry*) pEntry->GetNext();
				}
			}
			else
			{
				pHashData[i].dwNextIndex = 0;		// No more entries.
			}
            ZAssert( dwInitialAdd + dwNumChildren == dwAddIndex );
		}
	}

    ZAssert( dwAddIndex == dwTableSize );

	// Add to file.
	dwAmountWritten = m_pFile->Write( pHashData, dwTableSize * sizeof( SHashTableEntry ) );
    ZAssert( dwAmountWritten == dwTableSize * sizeof( SHashTableEntry ) );

	delete pHashData;
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// ImportPackFile()
//
////////////////////////////////////////////////////////////////////////////////
bool CDX9PackFile::ImportPackFile( )
{
	DWORD dwAmountRead;

	m_pFile	= new ZFile( m_szOutputFileName );
    ZAssert( m_pFile->IsValid() );

	m_pPackFile = new BYTE[ m_pFile->GetLength() ];
	dwAmountRead = m_pFile->Read( m_pPackFile, m_pFile->GetLength() );
    ZAssert( dwAmountRead == m_pFile->GetLength() );

	// Set up internal pointers.
	memcpy( &m_header, m_pPackFile, sizeof( SPackFileHeader ) );
	m_pHashEntryArray	= ( SHashTableEntry *) ( m_pPackFile + m_header.dwTableOffset );
	m_pRawFileData		= m_pPackFile + m_header.dwFileDataOffset;

	if( memcmp( m_header.szID, "AllegPak", 8 ) != 0 )
	{
		// 8/17/09 remove corrupt .pack file and carry on
        m_pFile->Release();
        delete m_pFile;
        m_pFile = NULL;
        DeleteFile((PCC)m_szOutputFileName);
        g_DX9Settings.mbUseTexturePackFiles = false;
		memset( &m_header, 0, sizeof( SHashTableEntry ) );
		delete[] m_pPackFile;
		m_pHashEntryArray = NULL;
		m_pRawFileData = NULL;
		m_pPackFile = NULL;
		
		return false;
	}

	delete m_pFile;
	m_pFile = NULL;
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// LoadFile()
//
////////////////////////////////////////////////////////////////////////////////
void * CDX9PackFile::LoadFileInternal( const char * szFileName, DWORD * pdwFileSize )
{
	void * pFilePtr = NULL;
	ZString szFile = szFileName;
	szFile = szFile.ToLower();

	DWORD dwHash = CHashTable::GenerateHashValue( &szFile[0] );
	DWORD dwIndex = dwHash % m_header.dwTableSize;

	SHashTableEntry * pEntry = &m_pHashEntryArray[ dwIndex ];
	while( pEntry != NULL )
	{
		if( pEntry->dwHashValue == dwHash )
		{
			// File found.
			pFilePtr = &m_pRawFileData[ pEntry->dwDataOffset ];
			*pdwFileSize = pEntry->dwFileSize;
			break;
		}
		if( pEntry->dwNextIndex == 0 )
		{
			pEntry = NULL;
		}
		else
		{
			pEntry = &m_pHashEntryArray[ pEntry->dwNextIndex ];
		}
	}

	if( ( pFilePtr == NULL ) && ( m_pNext != NULL ) )
	{
		pFilePtr = m_pNext->LoadFileInternal( szFileName, pdwFileSize );
	}
	return pFilePtr;
}


////////////////////////////////////////////////////////////////////////////////
// AddToPackFileList()
//
////////////////////////////////////////////////////////////////////////////////
void CDX9PackFile::AddToPackFileList( CDX9PackFile * pPackFile )
{
	CDX9PackFile * pList = m_pPackFileLinkedList;
	if( pList == NULL )
	{
		m_pPackFileLinkedList = pPackFile;
	}
	else
	{
		while( pList->m_pNext )
		{
			pList = pList->m_pNext;
		}
		pList->m_pNext = pPackFile;
	}
}


////////////////////////////////////////////////////////////////////////////////
// LoadFile()
//
////////////////////////////////////////////////////////////////////////////////
void * CDX9PackFile::LoadFile( const char * szFileName, DWORD * pdwFileSize )
{
	void * pFile = NULL;
	if( m_pPackFileLinkedList == NULL )
	{
		*pdwFileSize = 0;
	}
	else
	{
		pFile = m_pPackFileLinkedList->LoadFileInternal( szFileName, pdwFileSize );
	}
	return pFile;
}

