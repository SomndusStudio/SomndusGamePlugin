/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePercent = true;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	float Value;

	virtual void Initialize_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	float GetValue() const;
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float InValue);

	virtual TRange<double> GetSourceRange() const;
	virtual double GetSourceStep() const;

	virtual FText GetFormattedText_Implementation() override;
	
	float GetNormalizedStepSize();
	float GetValueNormalized();
};
