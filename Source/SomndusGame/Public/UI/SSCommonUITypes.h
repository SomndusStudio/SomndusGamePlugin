/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "SSCommonUITypes.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSCommonMessagingResultMCDelegate, ECommonMessagingResult, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSSInputActionExecutedDelegate, FName, ActionName);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_LAYER_MODAL);

/**
 * Encapsulates information required to define the cursor's appearance and behavior.
 *
 * This structure is utilized to request and represent cursor-related data, including
 * its position, size, and angle. It is primarily used within the UI system to manage
 * cursor attributes dynamically.
 */
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

/**
 * Represents menu information used within the system.
 *
 * This structure encapsulates details about a menu, including its
 * metadata and properties necessary for menu rendering and interaction.
 * It is designed to standardize the storage and handling of menu-specific data.
 */
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

	/**
	 * Platform visibility traits
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> VisibilityTraits;
};

/**
 * Represents a common UI item structure for defining UI elements in the system.
 *
 * This structure is used to encapsulate basic UI element data, including:
 * - An icon representing the UI item.
 * - A label text for the item.
 * - A description text for detailed information related to the item.
 */
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