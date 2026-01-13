/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "SSCommonSettingsHelpers.generated.h"

#define SS_GET_SUB_SETTINGS_FUNCTION_PATH(SubsetClass, SubsetGetter, FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(UGameUserSettings, SubsetGetter), \
GET_FUNCTION_NAME_STRING_CHECKED(SubsetClass, FunctionName) \
}))

class UTexture2D;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSSettingExtension_ImageThumbnail : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> DefaultThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UTexture2D>> Thumbnails;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSSettingExtension_VideoThumbnail : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DefaultVideoFilename;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> VideoFilenames;
};