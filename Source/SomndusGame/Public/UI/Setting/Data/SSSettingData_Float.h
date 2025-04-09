// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "UI/Setting/SSSettingDataObject.h"
#include "SSSettingData_Float.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingData_Float : public USSSettingDataObject
{
	GENERATED_BODY()

public:

	USSSettingData_Float();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue = 1.0f;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	float Value;

	UFUNCTION(BlueprintCallable)
	float GetValue();
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float InValue);

	virtual TRange<double> GetSourceRange() const;
	virtual double GetSourceStep() const;

	virtual FText GetFormattedText_Implementation() override;
	
	float GetNormalizedStepSize();
	float GetValueNormalized();
};
