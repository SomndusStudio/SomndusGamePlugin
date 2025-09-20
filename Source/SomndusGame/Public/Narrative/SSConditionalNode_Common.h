// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSNarrativeNode.h"
#include "SSConditionalNode_Common.generated.h"

UENUM(BlueprintType)
enum class ESSConditionType : uint8
{
	HasFlag,
	MissingFlag,
	HasGameplayTag,
	MissingGameplayTag
};

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSConditionalNode_Common : public USSNarrativeNode
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category="Condition")
	FName TargetProfile;
	
	// Type of condition to evaluate
	UPROPERTY(EditAnywhere, Category="Condition")
	ESSConditionType ConditionType = ESSConditionType::HasFlag;

	// The flag name to check in context
	UPROPERTY(EditAnywhere, Category="Condition")
	FName Flag;

	// Optional gameplay tag to check against context
	UPROPERTY(EditAnywhere, Category="Condition")
	FGameplayTag GameplayTag;

	virtual bool CanActivate_Implementation(USSNarrativeContext* Context) const override;
};
