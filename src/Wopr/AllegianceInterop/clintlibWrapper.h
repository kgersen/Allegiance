#pragma once

#include "clintlib.h"

using namespace System;

namespace AllegianceInterop
{
	public ref class MissionInfoWrapper
	{
	public:
		::MissionInfo * m_instance;

	public:
		MissionInfoWrapper(::MissionInfo * instance);

		String ^        Name() { return gcnew String(m_instance->Name()); }
		String ^        Description() { return gcnew String(m_instance->Description()); }
		int				GetCookie() { return m_instance->GetCookie(); }
	};
}