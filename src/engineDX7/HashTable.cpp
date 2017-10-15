#include "pch.h"
#include "HashTable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CHashTable()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
CHashTable::CHashTable( DWORD dwTableSize /*= s_iDefaultTableSize*/,
						bool bValidateEntries /*= false*/ )
{
	m_dwTableSize = dwTableSize;
	m_bValidateEntries = bValidateEntries;
	m_ppHashTable = new CHashEntry* [ m_dwTableSize ];
	m_dwTotalEntriesAdded = 0;

	// Reset the values.
	memset( m_ppHashTable, 0, m_dwTableSize * sizeof( CHashEntry * ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CHashTable()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
CHashTable::~CHashTable(void)
{
	CHashEntry * pCurrEntry, * pNextEntry;

	for( DWORD i=0; i<m_dwTableSize; i++ )
	{
		if( m_ppHashTable[i] != NULL )
		{
			pCurrEntry = m_ppHashTable[i];
			while( pCurrEntry != NULL )
			{
				pNextEntry = pCurrEntry->GetNext();
				if( pCurrEntry )
				{
					delete pCurrEntry;
					pCurrEntry = NULL;
				}
				pCurrEntry = pNextEntry;
			}
			m_ppHashTable[i] = NULL;
		}
	}

	if( m_ppHashTable )
	{
		delete [] m_ppHashTable;
		m_ppHashTable = NULL;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// AddHashEntry()
// Add the hash entry to the hash table. Returns false if a hash value clash occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////
CHashEntry * CHashTable::AddHashEntry( CHashEntry * pHashEntry )
{
	CHashEntry * pRetVal = NULL;
	DWORD dwTableIndex;
	
	dwTableIndex = pHashEntry->GetHashValue() % m_dwTableSize;

	if( m_bValidateEntries == true )
	{
		CHashEntry * pHashPtr = m_ppHashTable[ dwTableIndex ];
		while( pHashPtr != NULL )
		{
			if( pHashPtr->GetHashValue() == pHashEntry->GetHashValue() )
			{
				pRetVal = pHashPtr;
				break;
			}
			pHashPtr = pHashPtr->GetNext();
		}
	}

	// Add the entry anyway?
	if( pRetVal == NULL )
	{
		pHashEntry->SetNext( m_ppHashTable[ dwTableIndex ] );
		m_ppHashTable[ dwTableIndex ] = pHashEntry;
		m_dwTotalEntriesAdded ++;
	}

	return pRetVal;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetHashEntry()
// Returns a hash entry with a matching hash value, NULL if no match is found.
////////////////////////////////////////////////////////////////////////////////////////////////////
CHashEntry * CHashTable::GetHashEntry(DWORD dwHashSearchValue)
{
	DWORD dwTableIndex;
	CHashEntry * pRetVal = NULL;

	dwTableIndex = dwHashSearchValue % m_dwTableSize;
	pRetVal = m_ppHashTable[ dwTableIndex ];

	while( pRetVal != NULL )
	{
		if( pRetVal->GetHashValue() == dwHashSearchValue )
		{
			break;
		}
		pRetVal = pRetVal->GetNext();
	}
	return pRetVal;
}


////////////////////////////////////////////////////////////////////////////////////
// GetLeadHashEntryByIndex()
//
////////////////////////////////////////////////////////////////////////////////////
CHashEntry * CHashTable::GetLeadHashEntryByIndex( DWORD dwIndex )
{
	_ASSERT( dwIndex < m_dwTableSize );
	return m_ppHashTable[ dwIndex ];
}


////////////////////////////////////////////////////////////////////////////////////
// GetNumChildren()
//
////////////////////////////////////////////////////////////////////////////////////
DWORD CHashTable::GetNumChildren( CHashEntry * pEntry )
{
	DWORD dwNumChildren = 0;
	while( pEntry != NULL )
	{
		if( pEntry->GetNext() != NULL )
		{
			dwNumChildren ++;
		}
		pEntry = pEntry->GetNext();
	}
	return dwNumChildren;
}

////////////////////////////////////////////////////////////////////////////////////
// GenerateHashValue()
//
////////////////////////////////////////////////////////////////////////////////////
DWORD CHashTable::GenerateHashValue( const char * szString )
{
	DWORD dwHashValue = 0, dwChar;

	if( strlen( szString ) == 0 )
	{
		return 0;
	}

	dwHashValue = 5381;

	dwChar = (DWORD) * szString ++;

	while (dwChar != 0)
	{
		dwHashValue = ((dwHashValue << 5) + dwHashValue) + dwChar;			// ulHash * 33 + iChar
		dwChar = *szString ++;
	}

	return dwHashValue;
}



