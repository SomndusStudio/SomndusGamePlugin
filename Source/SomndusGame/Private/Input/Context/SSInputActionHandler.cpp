// ©2025 Schartier Isaac. All rights reserved.


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