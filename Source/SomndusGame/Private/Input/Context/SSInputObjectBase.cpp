// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSInputObjectBase.h"

#include "GameFramework/Character.h"
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

ACharacter* USSInputObjectBase::GetControlledCharacter()
{
	return Cast<ACharacter>(GetControlledPawn());
}

USSInputContextComponent* USSInputObjectBase::GetNativeInputComponent()
{
	return InputComponent;
}

USSInputContextComponent* USSInputObjectBase::GetInputComponent()
{
	return GetNativeInputComponent();
}
