#pragma once

using namespace System;

namespace AllegianceInterop {

	public interface class ISupportManagedByteArrays
	{
	public:
		array<unsigned char> ^ ToManagedByteArray();
	};

}