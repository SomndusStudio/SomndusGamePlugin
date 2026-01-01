/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UI/SSCommonUITypes.h"
#include "UI/SSTabButton.h"
#include "UI/SSTabListWidget.h"
#include "SSTabMenuNavigationWidget.generated.h"

class USSTabButton;
class UPanelWidget;
class UCommonButtonBase;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSTabMenuNavigationWidget : public USSTabListWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPreview = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonButtonBase> TabButtonClass = USSTabButton::StaticClass();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSMenuInfo> MenuEntries;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	FDataTableRowHandle PreviousTriggerAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	FDataTableRowHandle NextTriggerAction;
	
	UFUNCTION(BlueprintCallable)
	void SetupFromMenuEntries();
	
	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	virtual void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	virtual void NativeNotifyTabInfoSelected(FName TabId) override;
	
protected:

	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandlePreviousAction();

	UFUNCTION()
	void HandleNextAction();
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* EntryParentWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonButtonBase* PreviousTabActionButton;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonButtonBase* NextTabActionButton;
	
};
