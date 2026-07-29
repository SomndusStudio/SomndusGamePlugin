// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/SSCommonCharacter.h"

#include "Components/GameFrameworkComponentManager.h"


ASSCommonCharacter::ASSCommonCharacter()
{
}

void ASSCommonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, "SetupPlayerInputReady");
	
}

