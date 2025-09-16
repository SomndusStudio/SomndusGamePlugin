// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSPawnInputContext.h"

#include "SSLog.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Helper/SSHelperStatics.h"
#include "Input/Context/SSInputContextComponent.h"
#include "Input/Context/SSInputHandlerContext_Default.h"


void USSPawnInputContext::RegisterHandler(const FGameplayTag& InTag, USSInputHandlerContext* Handler)
{
	if (!Handler) return;

	InputHandlerContexts.Add(InTag, Handler);
	
	UE_LOG(LogSomndusGame, Log, TEXT("[USSPawnInputContext] Registered handler '%s' with tag '%s' in context '%s'"),
    		*GetNameSafe(Handler),
    		*InTag.ToString(),
    		*GetNameSafe(this));
}

void USSPawnInputContext::SetPawn(APawn* InPawn)
{
	// Avoid duplicate pawn assignation
	if (Pawn == InPawn) return;

	Pawn = InPawn;

	UE_LOG(LogSomndusGame, Log, TEXT("[USSPawnInputContext] Set pawn to %s"), *Pawn->GetName());
}

APawn* USSPawnInputContext::GetPawn()
{
	return Pawn;
}

USSInputContextComponent* USSPawnInputContext::GetInputContextComponent()
{
	return InputContextComponent;
}

USSInputHandlerContext* USSPawnInputContext::GetOrCreateInputHandlerContext(const FGameplayTag& InTag, const TSubclassOf<USSInputHandlerContext>& InClas)
{
	if (const auto* FoundInputContextHandler = InputHandlerContexts.Find(InTag))
	{
		return *FoundInputContextHandler;
	}

	auto* InputContextHandler = NewObject<USSInputHandlerContext>(this, InClas);
	InputContextHandler->NativeInit(this);
	InputContextHandler->SetContext(InTag);

	RegisterHandler(InTag, InputContextHandler);

	return InputContextHandler;
}

USSInputHandlerContext* USSPawnInputContext::PushInputContextDefinition(USSInputContextDefinition* InputContextDefinition)
{
	auto HandlerClass = USSHelperStatics::TryGetClass(InputContextDefinition->InputHandlerContextClass);
	auto* InputHandlerContext = GetOrCreateInputHandlerContext(InputContextDefinition->ContextTag, HandlerClass);
	auto* InputHandlerContext_Default = Cast<USSInputHandlerContext_Default>(InputHandlerContext);

	if (!InputHandlerContext_Default)
	{
		return InputHandlerContext;
	}

	for (const auto& Entry : InputContextDefinition->InputContextSettings)
	{
		auto InputAction = USSHelperStatics::TryGetAsset(Entry.InputAction);
		InputHandlerContext_Default->AddInputActionHandler(InputAction, Entry.InputActionHandlerClass);
	}

	return InputHandlerContext;
}

bool USSPawnInputContext::SetActiveInputContextFromTag(const FGameplayTag& InContextTag, bool bActive)
{
	auto* InputHandlerContext = GetOrCreateInputHandlerContext(InContextTag);
	if (!InputHandlerContext)
	{
		return false;
	}

	if (bActive)
	{
		InputHandlerContext->NativeActivate();
		ActiveInputHandlerContexts.AddUnique(InputHandlerContext);
	}
	else
	{
		InputHandlerContext->NativeDeactivate();
		ActiveInputHandlerContexts.Remove(InputHandlerContext);
	}

	// Reorder by priority (higher priority comes first)
	ActiveInputHandlerContexts.Sort([](const TObjectPtr<USSInputHandlerContext>& A, const TObjectPtr<USSInputHandlerContext>& B)
	{
		return A && B ? A->Priority > B->Priority : B != nullptr;
	});

	return true;
}

TArray<TObjectPtr<USSInputHandlerContext>> USSPawnInputContext::GetActiveInputHandlerContexts()
{
	return ActiveInputHandlerContexts;
}

void USSPawnInputContext::NativeInitialize(const TObjectPtr<USSInputContextComponent>& InInputContextComponent, const FGameplayTag& InContextTag)
{
	InputContextComponent = InInputContextComponent;
	Tag = InContextTag;

	UE_LOG(LogSomndusGame, Log, TEXT("USSPawnInputContext::NativeInitialize: Initialized with context tag [%s] and InputContextComponent [%s]."),
	       *Tag.ToString(),
	       *InputContextComponent->GetName());
}

void USSPawnInputContext::ActivateInputHandlers(UEnhancedInputComponent* InputComp)
{
}

void USSPawnInputContext::Deactivate(UEnhancedInputComponent* InputComp)
{
}
