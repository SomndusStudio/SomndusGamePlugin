/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "K2Node_SSCustomClassAccessBase.h"
#include "K2Node_SSGetDAO.generated.h"

/**
 * @class UK2Node_SSGetDAO
 * A node implementation used in Unreal Engine's Blueprint system to retrieve DAO (Data Access Object) instances.
 *
 * The UK2Node_SSGetDAO class is derived from UK2Node_SSCustomClassAccessBase and specializes in enabling
 * Blueprint access to objects that extend from USSDAOObjectBase. This allows users to dynamically create
 * and access DAO objects within the editor's visual scripting environment.
 */
UCLASS()
class SOMNDUSGAMEEDITOR_API UK2Node_SSGetDAO : public UK2Node_SSCustomClassAccessBase
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
