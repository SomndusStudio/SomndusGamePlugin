// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "UI/Setting/SSSettingDataObject.h"
#include "SSSettingData_Int.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingData_Int : public USSSettingDataObject
{
	GENERATED_BODY()

public:
	
	USSSettingData_Int();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Step = 1;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Value;

	virtual void Initialize_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	int32 GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable)
	void SetValue(int32 InValue);

	virtual FText GetFormattedText_Implementation() override;
};
