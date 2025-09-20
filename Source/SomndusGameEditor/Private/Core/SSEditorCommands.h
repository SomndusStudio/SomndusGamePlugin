/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Styling/AppStyle.h"
#include "Framework/Commands/Commands.h"

/**
 * 
 */
class SOMNDUSGAMEEDITOR_API FSSEditorCommands : public TCommands<FSSEditorCommands>
{
public:
	//ctor
	FSSEditorCommands()
		: TCommands<FSSEditorCommands>(TEXT("SSEditorCommands"), NSLOCTEXT("SSContexts", "SSEditorCommands", "SS Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> Compile;
	TSharedPtr<FUICommandInfo> SaveSelectedAsset;
};
