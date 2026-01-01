/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

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
	virtual bool OnInputActionCancel(const FInputActionInstance& InputActionInstance) override;
	virtual bool OnInputActionCompleted(const FInputActionInstance& InputActionInstance) override;

	template <typename T>
	T* GetInputActionHandlerByClass()
	{
		static_assert(TIsDerivedFrom<T, USSInputActionHandler>::IsDerived, "T must derive from USSInputActionHandler");

		for (const FSSInputActionHandlerSpec& Spec : InputActionHandlerSpecs)
		{
			if (T* Casted = Cast<T>(Spec.InputActionHandler))
			{
				return Casted;
			}
		}

		return nullptr;
	}
};
