/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonInputSubsystem.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSInputStaticsLibrary.generated.h"

class USSInputLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FText GetKeyNameForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FText GetKeyNameForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, bool bGamepad = false);

	UFUNCTION(BlueprintCallable)
	static bool IsKeyInInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, const UInputAction* OtherInputAction);
	
	static FKey GetFirstKeyForInputType(const UCommonInputSubsystem* CommonInputSubsystem, ECommonInputType InputType, const TArray<FDataTableRowHandle>& InputActions);
	
	UFUNCTION(BlueprintCallable)
	static FKey GetFirstKeyForInputType(const USSInputLocalPlayerSubsystem* SSInputLocalPlayer, ECommonInputType InputType, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, bool bGamepad = false);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForKey(const UCommonInputSubsystem* CommonInputSubsystem, const FKey Key);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(WorldContext="WorldContextObject"))
	static void SetGameSuspendInput(const UObject* WorldContextObject, FGameplayTag Tag, bool bSuspend=true);
};
