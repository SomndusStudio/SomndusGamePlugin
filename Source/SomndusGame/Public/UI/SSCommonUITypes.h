// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "SSCommonUITypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_LAYER_MODAL);

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSMenuInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Label;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Value;
};

/** For some UI */
USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSCommonUIItem : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Label;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};