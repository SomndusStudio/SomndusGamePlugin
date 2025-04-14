// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "UI/Setting/SSSettingDataObject.h"
#include "SSSettingData_Options.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingData_Options : public USSSettingDataObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Options;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	int32 IndexValue = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetValue();
	
	UFUNCTION(BlueprintCallable)
	void SetIndexValue(int32 InValue);
};
