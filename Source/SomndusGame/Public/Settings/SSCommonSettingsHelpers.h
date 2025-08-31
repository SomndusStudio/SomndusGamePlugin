// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameSettingRegistry.h"

#define SS_GET_SUB_SETTINGS_FUNCTION_PATH(SubsetClass, SubsetGetter, FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(UGameUserSettings, SubsetGetter), \
GET_FUNCTION_NAME_STRING_CHECKED(SubsetClass, FunctionName) \
}))