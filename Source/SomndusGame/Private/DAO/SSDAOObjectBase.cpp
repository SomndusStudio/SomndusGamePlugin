/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "DAO/SSDAOObjectBase.h"

#include "SSLog.h"

void USSDAOObjectBase::NativeInitialize()
{
	UE_LOG(LogSomndusGame, Display, TEXT("AFT: %s"), *GetName());
}
