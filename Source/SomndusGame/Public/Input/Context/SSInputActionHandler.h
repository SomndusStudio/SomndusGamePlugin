// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "SSInputObjectBase.h"
#include "SSInputActionHandler.generated.h"

/**
 * USSInputActionHandler is responsible for handling input actions in the game.
 * This is a base class specifically designed to manage and process input action events
 * like triggering, starting, and completing. It derives from USSInputObjectBase
 * and provides a foundation for implementing custom input action handling logic.
 */
UCLASS()
class SOMNDUSGAME_API USSInputActionHandler : public USSInputObjectBase
{
	GENERATED_BODY()

public:

	void Init(USSInputContextComponent* InInputComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool HandleInputActionTriggered(const FInputActionInstance& InputActionInstance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool HandleInputActionStarted(const FInputActionInstance& InputActionInstance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool HandleInputActionCompleted(const FInputActionInstance& InputActionInstance);
};
