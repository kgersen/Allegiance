#include "stdafx.h"
#include "CriticalSectionManager.h"

namespace AllegianceInterop
{

	CriticalSectionManager::CriticalSectionManager()
	{
		::InitializeCriticalSection(&m_CS);
	}
	CriticalSectionManager::~CriticalSectionManager()
	{
		::DeleteCriticalSection(&m_CS);
	}
	void CriticalSectionManager::Lock()
	{
		::EnterCriticalSection(&m_CS);
	}
	void CriticalSectionManager::UnLock()
	{
		::LeaveCriticalSection(&m_CS);
	}
}