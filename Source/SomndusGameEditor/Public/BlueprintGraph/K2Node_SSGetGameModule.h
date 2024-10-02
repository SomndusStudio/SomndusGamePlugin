// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_SSCustomClassAccessBase.h"
#include "K2Node_SSGetGameModule.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEEDITOR_API UK2Node_SSGetGameModule : public UK2Node_SSCustomClassAccessBase
{
	GENERATED_BODY()

public:

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual FText GetTooltipText() const override;
	
	virtual void CreateInputCustomPin() override;
	virtual void CreateOutputCustomPin() override;

	virtual FName GetFunctionAccessName() override;

	virtual TSubclassOf<UObject> GetOutputClassObject() override;
};
