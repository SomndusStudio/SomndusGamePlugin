/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "K2Node_SSCustomClassAccessBase.h"
#include "K2Node_SSGetGameModule.generated.h"

/**
 * @class UK2Node_SSGetGameModule
 * A specialized K2Node designed to access and retrieve game modules based on a custom class.
 *
 * This class is derived from the `UK2Node_SSCustomClassAccessBase` and serves as
 * a Blueprint node that facilitates obtaining instances of specific game modules in the editor.
 */
UCLASS()
class SOMNDUSGAMEEDITOR_API UK2Node_SSGetGameModule : public UK2Node_SSCustomClassAccessBase
{
	GENERATED_BODY()

public:

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual FText GetTooltipText() const override;
	
	virtual void CreateInputCustomPin() override;
	virtual void CreateOutputCustomPin() override;

	virtual FName GetFunctionAccessName() override;

	virtual TSubclassOf<UObject> GetOutputClassObject() override;
};
