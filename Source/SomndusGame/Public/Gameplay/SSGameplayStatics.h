// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Somndus Studio|Gameplay", meta = (ExpandBoolAsExecs))
	static bool Actor_IsPrimaryPlayer(AActor* Actor);
};
