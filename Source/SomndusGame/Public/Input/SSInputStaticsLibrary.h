// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputSubsystem.h"
#include "InputAction.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSInputStaticsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static FText GetKeyNameForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction);
};
