// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSInputHandlerContext_Default.h"

#include "SSLog.h"
#include "Helper/SSHelperStatics.h"
#include "Input/Context/SSInputActionHandler.h"
#include "Input/Context/SSPawnInputContext.h"

USSInputActionHandler* USSInputHandlerContext_Default::AddInputActionHandler(TObjectPtr<UInputAction> InputAction,
                                                                             TSoftClassPtr<USSInputActionHandler> InputActionHandlerClass)
{
	if (!InputAction)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("AddInputActionHandler: InputAction is null."));
		return nullptr;
	}
	
	auto LoadedInputActionHandlerClass = USSHelperStatics::TryGetClass(InputActionHandlerClass);
	if (!LoadedInputActionHandlerClass)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("AddInputActionHandler: Failed to load class from TSoftClassPtr <%s>."), *InputActionHandlerClass.ToString());
		return nullptr;
	}
	
	auto* InputActionHandler = NewObject<USSInputActionHandler>(this, LoadedInputActionHandlerClass);
	if (!InputActionHandler)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("AddInputActionHandler: Failed to create instance of class <%s>."), *LoadedInputActionHandlerClass->GetName());
		return nullptr;
	}
	
	InputActionHandler->Init(PawnInputContext->GetInputContextComponent());
	
	FSSInputActionHandlerSpec Spec;
	Spec.InputAction = InputAction;
	Spec.InputActionHandler = InputActionHandler;
	InputActionHandlerSpecs.Add(Spec);

	UE_LOG(LogSomndusGame, Log, TEXT("AddInputActionHandler: Successfully registered handler <%s> for input action <%s>."),
	 *InputActionHandler->GetClass()->GetName(),
	 *InputAction->GetName());
	
	return InputActionHandler;
}

bool USSInputHandlerContext_Default::OnInputActionStarted(const FInputActionInstance& InputActionInstance)
{
	for (const auto& Spec: InputActionHandlerSpecs)
	{
		if (Spec.InputAction != InputActionInstance.GetSourceAction())
		{
			continue;
		}
		if (Spec.InputActionHandler->HandleInputActionStarted(InputActionInstance))
		{
			return true;
		}
	}
	return false;
}

bool USSInputHandlerContext_Default::OnInputActionTrigger(const FInputActionInstance& InputActionInstance)
{
	for (const auto& Spec: InputActionHandlerSpecs)
	{
		if (Spec.InputAction != InputActionInstance.GetSourceAction())
		{
			continue;
		}
		if (Spec.InputActionHandler->HandleInputActionTriggered(InputActionInstance))
		{
			return true;
		}
	}
	return false;
}

bool USSInputHandlerContext_Default::OnInputActionCompleted(const FInputActionInstance& InputActionInstance)
{
	for (const auto& Spec: InputActionHandlerSpecs)
	{
		if (Spec.InputAction != InputActionInstance.GetSourceAction())
		{
			continue;
		}
		if (Spec.InputActionHandler->HandleInputActionCompleted(InputActionInstance))
		{
			return true;
		}
	}
	return false;
}