/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/DynamicEntryBoxBase.h"
#include "SSBoundGameActionBar.generated.h"

class USSGameInputActionWrapperWidget;
class USSGameInputActionWidget;
class UCommonButtonBase;

/**
 * Data structure that defines a single bound input action to display in the action bar.
 */
USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSBoundGameActionInfo
{
	GENERATED_BODY()

	/** Input Action to bind and display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction;

	/** Whether to override the default display name of the action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideInputActionDisplayName;
	
	/** Custom display name (used only if override is enabled) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
};

/**
 * Displays a dynamic list of USSGameInputActionWidget based on a list of input actions.
 */
UCLASS()
class SOMNDUSGAME_API USSBoundGameActionBar : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	/**
	 * Sets and displays the given list of bound actions.
	 * @param InActions List of actions to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetBoundActions(const TArray<FSSBoundGameActionInfo>& InActions);

protected:
	
	/** Called when the widget is constructed */
	virtual void Initialize();

	/** Rebuilds the list of action widgets */
	void RebuildActionWidgets();
	
private:
	/** The widget class used for each input action entry */
	UPROPERTY(EditAnywhere, Category = EntryLayout)
	TSubclassOf<USSGameInputActionWrapperWidget> InputActionWidgetClass;

	/** If true, the same action won't be displayed more than once */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Display)
	bool bIgnoreDuplicateActions = true;

	/** Cached list of currently bound actions */
	UPROPERTY(Transient)
	TArray<FSSBoundGameActionInfo> BoundActions;
};
