/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"

#include "SSSettingWidgetThemeDataAsset.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSWidgetSettingThemeInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> SoftWidgetClass = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSSettingWidgetThemeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSWidgetSettingThemeInfo> SettingThemeInfos;

	FSSWidgetSettingThemeInfo DefaultSettingThemeInfo;
	
	FSSWidgetSettingThemeInfo& GetSettingThemeInfo(const FGameplayTag& Tag);
};
