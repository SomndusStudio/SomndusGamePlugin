// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSNarrativeTypes.h"
#include "SSContextObject.h"
#include "SSNarrativeContext.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSNarrativeContext : public USSContextObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ActiveNarrativeTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSNarrativeProfile NarrativeProfileFallback;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSNarrativeProfile> NarrativeProfiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Flags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;
	
	FSSNarrativeProfile* GetProfile(FName ProfileId);

	void LoadFromSaveData(const FSSNarrativeSaveData* SaveData);
	FSSNarrativeSaveData GetSave() const;
};
