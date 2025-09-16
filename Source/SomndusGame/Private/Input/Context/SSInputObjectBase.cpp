// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSInputObjectBase.h"

#include "GameFramework/PlayerController.h"
#include "Input/Context/SSInputContextComponent.h"

AController* USSInputObjectBase::GetController()
{
	return Cast<AController>(GetInputComponent()->GetOwner());
}

APlayerController* USSInputObjectBase::GetPlayerController()
{
	return GetInputComponent()->GetPlayerController();
}

APawn* USSInputObjectBase::GetControlledPawn()
{
	return GetPlayerController()->GetPawn();
}

USSInputContextComponent* USSInputObjectBase::GetNativeInputComponent()
{
	return InputComponent;
}

USSInputContextComponent* USSInputObjectBase::GetInputComponent()
{
	return GetNativeInputComponent();
}
