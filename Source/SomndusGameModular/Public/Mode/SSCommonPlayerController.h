// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SSCommonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSCommonPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
protected:

	ASSCommonPlayerController();

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	
public:

	UPROPERTY(DuplicateTransient)
	TObjectPtr<class USSInputContextComponent> InputContextComponent;

};
