// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "SSSettingWidgetThemeDataAsset.generated.h"

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
