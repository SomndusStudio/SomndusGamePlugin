/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#include "SSEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSSEditorModule"

/** UI_COMMAND takes long for the compile to optimize */
UE_DISABLE_OPTIMIZATION
void FSSEditorCommands::RegisterCommands()
{
	UI_COMMAND(Compile, "Compile", "Compile the blueprint", EUserInterfaceActionType::Button, FInputChord());
	
	UI_COMMAND(SaveSelectedAsset, "Save", "Save the selected asset(s).", EUserInterfaceActionType::Button, FInputChord(EKeys::S, EModifierKey::Control));
}

UE_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
