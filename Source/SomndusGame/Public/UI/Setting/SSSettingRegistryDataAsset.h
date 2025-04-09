// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSSettingWidgetObject.h"
#include "Engine/DataAsset.h"
#include "SSSettingRegistryDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSSettingRegistryDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<USSSettingWidgetObject>> Settings;
};
