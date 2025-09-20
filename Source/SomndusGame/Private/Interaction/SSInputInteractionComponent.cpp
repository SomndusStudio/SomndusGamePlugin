// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/SSInputInteractionComponent.h"


USSInputInteractionComponent::USSInputInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USSInputInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractionMgr = NewObject<USSInteractionInputMgr>(this);
}

