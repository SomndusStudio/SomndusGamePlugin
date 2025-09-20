// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSInteractionInputMgr.h"
#include "Components/ActorComponent.h"
#include "SSInputInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAME_API USSInputInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USSInputInteractionComponent();

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSInteractionInputMgr> InteractionMgr;

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	USSInteractionInputMgr* GetInteractionMgr() { return InteractionMgr; }
};
