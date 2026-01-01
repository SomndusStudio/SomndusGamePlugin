/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UI/SSGameActivatableWidget.h"
#include "SSGameNavigableScreen.generated.h"

class USSTabListWidget;
class UWidgetSwitcher;
class USSTabMenuNavigationWidget;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameNavigableScreen : public USSGameActivatableWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
		
	UFUNCTION()
	void NativeOnTopNavigationChange(USSTabListWidget* Widget, int32 Index, FName Info);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnTopNavigationChange(USSTabMenuNavigationWidget* Widget, int32 Index, FName Info);
	
protected:

	UPROPERTY(meta = (BindWidget))
	USSTabMenuNavigationWidget* TopNavigationWidget;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcherContent;

};
