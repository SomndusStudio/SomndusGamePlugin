// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSContextObject.h"
#include "SSSettingDataHandler.h"
#include "SSSettingWidgetObject.generated.h"

class USSSettingDataObject;
/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SOMNDUSGAME_API USSSettingWidgetObject : public USSContextObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Identifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TypeTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<USSSettingDataObject> Data;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<USSSettingDataHandler> Handler;
};
