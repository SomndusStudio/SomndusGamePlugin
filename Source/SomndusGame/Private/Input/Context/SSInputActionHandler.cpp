/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/Context/SSInputActionHandler.h"


void USSInputActionHandler::Init(USSInputContextComponent* InInputComponent)
{
	InputComponent = InInputComponent;
	OnInit();
}

void USSInputActionHandler::OnInit_Implementation()
{
}

bool USSInputActionHandler::HandleInputActionTriggered_Implementation(const FInputActionInstance& InputActionInstance)
{
	return false;
}

bool USSInputActionHandler::HandleInputActionStarted_Implementation(const FInputActionInstance& InputActionInstance)
{
	return false;
}

bool USSInputActionHandler::HandleInputActionCancel_Implementation(const FInputActionInstance& InputActionInstance)
{
	return false;
}

bool USSInputActionHandler::HandleInputActionCompleted_Implementation(const FInputActionInstance& InputActionInstance)
{
	return false;
}