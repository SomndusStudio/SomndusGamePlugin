// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSNarrativeNode.h"
#include "SSNarrativeNode_Start.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "NarrativeNode (Start)")
class SOMNDUSGAME_API USSNarrativeNode_Start : public USSNarrativeNode
{
	GENERATED_BODY()

public:
	USSNarrativeNode_Start();

	virtual FText GetDisplayName_Implementation() const override;
};
