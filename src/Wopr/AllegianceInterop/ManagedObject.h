#pragma once
#include "messagecore.h"
#include "NativeClient.h"
#include "SimpleClient.h"
#include "ISupportManagedByteArrays.h"

using namespace System;
using namespace System::Runtime::InteropServices;

// From samples at: https://www.red-gate.com/simple-talk/dotnet/net-development/creating-ccli-wrapper/
namespace AllegianceInterop {

	//public struct DummyClass
	//{
	//public: 
	//	int cbmsg;
	//};

	
	template<class T>
	[Serializable]
	public ref class ManagedObject : public ISupportManagedByteArrays
	{
	private:
		[NonSerialized]
		GCHandle m_pinnedBytes;

		bool m_isInstanceGloballyAlloced = false;

		static long m_instanceCount = 0;

		System::Collections::Generic::List<IntPtr> ^ m_allocatedHandles = gcnew System::Collections::Generic::List<IntPtr>();

	protected:

		
		T * m_Instance = nullptr;
		
	public:

		ManagedObject() 
		{
			m_instanceCount++;

			// Always assume that if the default constructor is used, then the subclass is going to assign an object 
			// created with BEGIN_PFM_CREATE_ALLOC which will require PFM_DEALLOC to clean up.
			m_isInstanceGloballyAlloced = true;

			// Calling the template methods forces the compiler to include them in the managed assembly
			// otherwise they are omitted from the assembly if nothing in the assembly calls them. 
			//SpinAllTemplateMethods();
		}

		ManagedObject(array<System::Byte> ^ bytes)
		{
			m_instanceCount++;

			// Calling the template methods forces the compiler to include them in the managed assembly
			// otherwise they are omitted from the assembly if nothing in the assembly calls them. 
			//SpinAllTemplateMethods();

			m_pinnedBytes = GCHandle::Alloc(bytes, GCHandleType::Pinned);
			m_Instance = (T *)m_pinnedBytes.AddrOfPinnedObject().ToPointer();
		}

		void CleanUpAllocatedItems()
		{
			m_instanceCount--;

			if (m_Instance != nullptr && m_isInstanceGloballyAlloced == true)
				PFM_DEALLOC(m_Instance);

			m_Instance = nullptr;

			if (m_pinnedBytes.IsAllocated == true)
				m_pinnedBytes.Free();

			for each(IntPtr allocatedHandle in m_allocatedHandles)
			{
				Marshal::FreeHGlobal(allocatedHandle);
			}

			m_allocatedHandles->Clear();
			
		}

		virtual ~ManagedObject()
		{
			CleanUpAllocatedItems();
		}

		!ManagedObject()
		{
			CleanUpAllocatedItems();
		}

		T* GetInstance()
		{
			return m_Instance;
		}

		virtual array<unsigned char> ^ ToManagedByteArray()
		{
			if (m_Instance == nullptr)
				return gcnew array<unsigned char>(0);

			int cb = m_Instance->cbmsg;
			array<unsigned char> ^ returnValue = gcnew array<unsigned char>(cb);
			
			Marshal::Copy((IntPtr)m_Instance, returnValue, 0, cb);

			return returnValue;
		}

		const char * StringToCharArray(String ^ value)
		{
			IntPtr allocatedCharArray = Marshal::StringToHGlobalAnsi(value);
			
			m_allocatedHandles->Add(allocatedCharArray);
	
			return (const char*)allocatedCharArray.ToPointer();
		}

	protected:

		// This dummy method is used to force the compiler to include base class template methods
		// into the output managed c++/cli assembly so that we can call them from .Net.
		void SpinAllTemplateMethods()
		{
			StringToCharArray(gcnew String("yay"));
			ToManagedByteArray();
			//ISupportManagedByteArrays ^ i = gcnew ManagedObject<DummyClass>();
		}
	};
}