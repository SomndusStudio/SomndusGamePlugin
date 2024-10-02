// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSContextObject.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSContextObject : public UObject
{
	GENERATED_BODY()
	virtual void PostLoad() override;

	class ULevel* GetLevel() const;

public:
	// Allows the Object to use BP_Functions
	virtual class UWorld* GetWorld() const override;


	UFUNCTION(BlueprintPure)
	class UGameInstance* GetGameInstance() const;
	
};
