#pragma once

#include "messagecore.h"
#include "Messages.h"
#include "messageslc.h"
#include "messagesls.h"
#include "messagesall.h"

// Defines g_rgszMsgNames
// ALLOC_MSG_LIST;

#include "AllMessages.h"

using namespace System;
using namespace System::Reflection;

//namespace AllegianceInterop {
//	public ref class HelloWorld
//	{
//	public:
//
//		const char * SayHello()
//		{
//			return "Hello Pork!";
//		}
//
//		//AllegianceInterop::FMD_S_PLAYERINFO ^ GetPlayerInfo()
//		void GetPlayerInfo()
//		{
//			/*::FMD_S_PLAYERINFO * realValue = new ::FMD_S_PLAYERINFO();
//
//			strcpy(realValue->CharacterName, "SuperPork9000");
//
//			AllegianceInterop::FMD_S_PLAYERINFO ^ returnValue(realValue);
//
//			return returnValue;*/
//
//			//System::Type ^ type = System::Type::GetType("FMD_S_PLAYERINFO");
//
//			//array<System::Reflection::FieldInfo ^> ^ fields = type->GetFields( System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance);
//
//			//FMD_S_PLAYERINFO returnValue;
//			//
//
//			//strcpy(returnValue.CharacterName, "SuperPork9000");
//
//			////return &returnValue;
//			//return returnValue;
//		}
//
//	};
//}
