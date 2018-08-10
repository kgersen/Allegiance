#pragma once

#include <Windows.h>

namespace AllegianceInterop
{
	// https://stackoverflow.com/questions/800383/what-is-the-difference-between-mutex-and-critical-section
	public class CriticalSectionManager
	{
	private:
		CRITICAL_SECTION m_CS;
	public:
		CriticalSectionManager();
		~CriticalSectionManager();
		void Lock();
		void UnLock();
	};
}

