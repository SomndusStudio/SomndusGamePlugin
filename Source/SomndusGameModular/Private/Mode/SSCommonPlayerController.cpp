// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/SSCommonPlayerController.h"

#include "SSGameModularTypes.h"
#include "SSLog.h"
#include "Input/Context/SSInputContextComponent.h"

ASSCommonPlayerController::ASSCommonPlayerController()
{
}

void ASSCommonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (InputContextComponent == NULL)
	{
		InputContextComponent = NewObject<USSInputContextComponent>(this, USSInputContextComponent::StaticClass(), TEXT("PC_InputContextComponent"));
		InputContextComponent->RegisterComponent();

		UE_LOG(LogSomndusInput, Log, TEXT("%s : Setup PC_InputContextComponent"), *GetNameSafe(this));
	}
}

void ASSCommonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (InputContextComponent && GetPawn())
	{
		InputContextComponent->Init(this);
		SSGameModularHelpers::SendGameFrameworkComponentExtensionEvent(this, USSInputContextComponent::NAME_InputContextReady);
	}
	
	InputContextComponent->OnPawnChanged(InPawn);
}
