/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "SSCommonUITypes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSCommonMessagingResultMCDelegate, ECommonMessagingResult, Result);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_LAYER_MODAL);

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSCursorRequestInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Size = FVector2D(50.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = -90.f;
};

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