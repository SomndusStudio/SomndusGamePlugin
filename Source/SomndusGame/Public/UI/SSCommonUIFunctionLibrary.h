// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSCommonUIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SS|UI")
	static void SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject);
};
