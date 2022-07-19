#include "stdafx.h"
#include "clintlibWrapper.h"

using namespace System;

namespace AllegianceInterop
{
	MissionInfoWrapper::MissionInfoWrapper(::MissionInfo * instance)
	{
		m_instance = instance;
	}

}