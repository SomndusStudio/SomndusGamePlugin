// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

#pragma once

#include "CoreMinimal.h"
#include "SSInteractable.h"
#include "SSInteractionTypes.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "SSInteractableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAME_API USSInteractableComponent : public USceneComponent, public ISSInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USSInteractableComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSInteractionPrompt> Prompts;

	UPROPERTY(BlueprintReadWrite)
	TArray<FSSInteractionContext> CachedPromptContexts;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	void SetActiveInteraction(bool bInActive = true);

	UFUNCTION(BlueprintCallable)
	void PushInteractionContexts();

	UFUNCTION(BlueprintCallable)
	void RemoveInteractionContexts();
	
	virtual void OnInteract_Implementation(const FSSInteractionContext& SourceInteractionContext) override;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="EventDispatchers")
	FSSInteractionEvent OnInteractionSuccess;
};
