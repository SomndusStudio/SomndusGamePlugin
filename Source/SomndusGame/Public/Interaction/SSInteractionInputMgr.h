﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSInteractionTypes.h"
#include "SSInteractionInputMgr.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSInteractionContextEvent, class USSInteractionInputMgr*, InteractionInputMgr, FSSInteractionContext, InteractionContext);

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInteractionInputMgr : public USSContextObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FSSInteractionContext> AwaitInteractionContexts;

public:

	UFUNCTION(BlueprintCallable)
	bool PushInteractionContext(const FSSInteractionContext& InteractionContext);

	UFUNCTION(BlueprintCallable)
	bool RemoveInteractionContext(const FSSInteractionContext& InteractionContext);
	
	UFUNCTION(BlueprintCallable)
	bool CheckInputInteraction(UInputAction* EnhancedInputAction);
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="EventDispatchers")
	FSSInteractionContextEvent OnInteractionContextPush;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="EventDispatchers")
	FSSInteractionContextEvent OnInteractionContextRemove;
};
