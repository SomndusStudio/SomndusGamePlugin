// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonUITypes.h"
#include "SSTabButton.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSTabButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSMenuInfo MenuInfo;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMenuInfo(FSSMenuInfo InMenuInfo);
};
