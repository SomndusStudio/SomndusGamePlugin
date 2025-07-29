// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSSettingDataObject.generated.h"

class USSSettingDataHandler;

/**
* DELEGATES
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSSettingDataEventSignature, class USSSettingDataObject*, SettingDataObject);

UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SOMNDUSGAME_API USSSettingDataObject : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FName Identifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<USSSettingDataHandler> Handler;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Initialize();
	
	UFUNCTION(BlueprintCallable)
	void NotifyDefaultValueChange();

	UFUNCTION(BlueprintCallable)
	void NotifyValueChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetFormattedText();

	UPROPERTY(BlueprintAssignable)
	FSSSettingDataEventSignature OnDefaultValueChange;

	UPROPERTY(BlueprintAssignable)
	FSSSettingDataEventSignature OnValueChange;
};
