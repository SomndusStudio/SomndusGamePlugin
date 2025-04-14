// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSSettingDataObject.generated.h"

/**
* DELEGATES
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSSettingDataEventSignature, class USSSettingDataObject*, SettingDataObject);

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SOMNDUSGAME_API USSSettingDataObject : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FName Identifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayLabel;

	UFUNCTION(BlueprintCallable)
	void NotifyDefaultValueChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetFormattedText();

	UPROPERTY(BlueprintAssignable)
	FSSSettingDataEventSignature OnDefaultValueChange;
};
