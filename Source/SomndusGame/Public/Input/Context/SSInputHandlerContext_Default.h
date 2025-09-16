﻿// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSInputHandlerContext.h"
#include "SSInputHandlerContext_Default.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputHandlerContext_Default : public USSInputHandlerContext
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FSSInputActionHandlerSpec> InputActionHandlerSpecs;

	USSInputActionHandler* AddInputActionHandler(TObjectPtr<UInputAction> InputAction, TSoftClassPtr<USSInputActionHandler> InputActionHandlerClass);
	
	virtual bool OnInputActionStarted(const FInputActionInstance& InputActionInstance) override;
	virtual bool OnInputActionTrigger(const FInputActionInstance& InputActionInstance) override;
	virtual bool OnInputActionCompleted(const FInputActionInstance& InputActionInstance) override;
};
