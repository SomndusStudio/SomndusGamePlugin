// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com


#include "Interaction/SSInteractableComponent.h"

#include "Interaction/SSInputInteractionComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USSInteractableComponent::USSInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USSInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto& Prompt : Prompts)
	{
		FSSInteractionContext InteractionContext;
		InteractionContext.Object = this;
		InteractionContext.Prompt = Prompt;
		CachedPromptContexts.Add(InteractionContext);
	}
}

void USSInteractableComponent::SetActiveInteraction(bool bInActive)
{
	bActive = bInActive;
}

void USSInteractableComponent::PushInteractionContexts()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (USSInputInteractionComponent* InputInteractionComponent = PlayerController->FindComponentByClass<USSInputInteractionComponent>())
		{
			for (const auto& PromptContext : CachedPromptContexts)
			{
				InputInteractionComponent->GetInteractionMgr()->PushInteractionContext(PromptContext);
			}
		}
	}
}

void USSInteractableComponent::RemoveInteractionContexts()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (USSInputInteractionComponent* InputInteractionComponent = PlayerController->FindComponentByClass<USSInputInteractionComponent>())
		{
			for (const auto& PromptContext : CachedPromptContexts)
			{
				InputInteractionComponent->GetInteractionMgr()->RemoveInteractionContext(PromptContext);
			}
		}
	}
}

void USSInteractableComponent::OnInteract_Implementation(const FSSInteractionContext& SourceInteractionContext)
{
	RemoveInteractionContexts();
	OnInteractionSuccess.Broadcast(SourceInteractionContext);
}
