// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSInputHandlerContext.h"

#include "SSLog.h"
#include "Engine/LocalPlayer.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Input/Context/SSPawnInputContext.h"


ULocalPlayer* USSInputHandlerContext::GetOwningLocalPlayer() const
{
	if (PlayerContext.IsValid())
	{
		return PlayerContext.GetLocalPlayer();
	}
	return nullptr;
}

UCommonInputSubsystem* USSInputHandlerContext::GetInputSubsystem() const
{
	const ULocalPlayer* BindingOwner = GetOwningLocalPlayer();
	return UCommonInputSubsystem::Get(BindingOwner);
}

void USSInputHandlerContext::NativeInit(USSPawnInputContext* InPawnInputContext)
{
	PawnInputContext = InPawnInputContext;
	
	PlayerContext = FLocalPlayerContext(USSInputStaticsLibrary::GetLocalPlayer(this), GetWorld());
	
	OnInitialized();
}

void USSInputHandlerContext::NativeActivate()
{
	bIsActive = true;
}

void USSInputHandlerContext::NativeDeactivate()
{
	bIsActive = false;
}

void USSInputHandlerContext::OnInitialized_Implementation()
{
}

void USSInputHandlerContext::SetContext(const FGameplayTag& InContextTag)
{
	ContextTag = InContextTag;
}

APawn* USSInputHandlerContext::GetPawn()
{
	return PawnInputContext->GetPawn();
}

bool USSInputHandlerContext::InternalInputActionStarted(const FInputActionInstance& InputActionInstance)
{
	UE_LOG(LogSomndusGame, Verbose, TEXT("[%s] Input Action Started: %s"), *GetNameSafe(this), *InputActionInstance.GetSourceAction()->GetName());
	return OnInputActionStarted(InputActionInstance);
}

bool USSInputHandlerContext::InternalInputActionTrigger(const FInputActionInstance& InputActionInstance)
{
	UE_LOG(LogSomndusGame, Verbose, TEXT("[%s] Input Action Triggered: %s"), *GetNameSafe(this), *InputActionInstance.GetSourceAction()->GetName());
	return OnInputActionTrigger(InputActionInstance);
}

bool USSInputHandlerContext::InternalInputActionCompleted(const FInputActionInstance& InputActionInstance)
{
	UE_LOG(LogSomndusGame, Verbose, TEXT("[%s] Input Action Completed: %s"), *GetNameSafe(this), *InputActionInstance.GetSourceAction()->GetName());
	return OnInputActionCompleted(InputActionInstance);
}

bool USSInputHandlerContext::OnInputActionStarted(const FInputActionInstance& InputActionInstance)
{
	return false;
}

bool USSInputHandlerContext::OnInputActionTrigger(const FInputActionInstance& InputActionInstance)
{
	return false;
}

bool USSInputHandlerContext::OnInputActionCompleted(const FInputActionInstance& InputActionInstance)
{
	return false;
}

