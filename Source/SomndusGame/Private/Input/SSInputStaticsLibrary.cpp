﻿/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputStaticsLibrary.h"

#include "CommonUITypes.h"

FText USSInputStaticsLibrary::GetKeyNameForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem,
                                                               const UInputAction* InputAction)
{
	const FKey FirstKeyForCurrentInput = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), CommonInputSubsystem->GetCurrentInputType(), InputAction);

	if (FirstKeyForCurrentInput.IsValid()){
		return FirstKeyForCurrentInput.GetDisplayName();
	}

	return FText::FromString("");
}
