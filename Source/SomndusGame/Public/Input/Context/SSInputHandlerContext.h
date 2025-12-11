// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "SSContextObject.h"
#include "Engine/LocalPlayer.h"
#include "SSInputHandlerContext.generated.h"

class USSInputActionHandler;
class UCommonInputSubsystem;
class USSPawnInputContext;
class UInputAction;
class UEnhancedInputComponent;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputActionHandlerSpec
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction = nullptr;
	
	UPROPERTY( BlueprintReadWrite)
	int32 InputActionId;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSInputActionHandler> InputActionHandler = nullptr;
};


UCLASS()
class SOMNDUSGAME_API USSInputHandlerContext : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ContextTag;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Priority;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSPawnInputContext> PawnInputContext;

	/** The player context that is associated with this Input */
	FLocalPlayerContext PlayerContext;

	ULocalPlayer* GetOwningLocalPlayer() const;
	UCommonInputSubsystem* GetInputSubsystem() const;
	
public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsActive = false;

	UFUNCTION(BlueprintPure)
	bool IsActive() const { return bIsActive; }

	void NativeInit(USSPawnInputContext* InPawnInputContext);
	
	// Called to bind all input actions
	void NativeActivate();

	// Unbind all actions
	void NativeDeactivate();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInitialized();

	void SetContext(const FGameplayTag& InContextTag);
	
	UFUNCTION(BlueprintPure)
	APawn* GetPawn();

	bool InternalInputActionStarted(const FInputActionInstance& InputActionInstance);
	bool InternalInputActionTrigger(const FInputActionInstance& InputActionInstance);
	bool InternalInputActionCancel(const FInputActionInstance& InputActionInstance);
	bool InternalInputActionCompleted(const FInputActionInstance& InputActionInstance);
	
	virtual bool OnInputActionStarted(const FInputActionInstance& InputActionInstance);
	virtual bool OnInputActionTrigger(const FInputActionInstance& InputActionInstance);
	virtual bool OnInputActionCancel(const FInputActionInstance& InputActionInstance);
	virtual bool OnInputActionCompleted(const FInputActionInstance& InputActionInstance);

};
