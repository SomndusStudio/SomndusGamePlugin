/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "UI/SSCommonFocusWidget.h"
#include "UI/SSWidgetObjectEntry.h"

#include "SSCommonWidgetSlotEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSWidgetSlotEntryEvent, class USSCommonWidgetSlotEntry*, SlotEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSWidgetSlotEntryBoolEvent, class USSCommonWidgetSlotEntry*, SlotEntry, bool, bActive);

class USSBackgroundSlotWidget;

/**
 * Represents a common widget slot entry that holds an associated object and manages widget focus and interaction.
 * Extends functionality from UCommonUserWidget, ISSCommonFocusWidget, and ISSWidgetObjectEntry.
 */
UCLASS()
class SOMNDUSGAME_API USSCommonWidgetSlotEntry : public UCommonUserWidget, public ISSCommonFocusWidget, public ISSWidgetObjectEntry
{
	GENERATED_BODY()

public:

	USSCommonWidgetSlotEntry();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> ObjectDataType;
	
	UPROPERTY(BlueprintReadWrite)
	UObject* ItemObject;

	UFUNCTION(BlueprintPure)
	UObject* GetItemObject() const { return ItemObject; }
	
	virtual void OnItemObjectSet_Implementation(UObject* InItemObject) override;
	
	virtual const UWidget* GetDefaultFocusWidget_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreOnItemObjectUpdate();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnItemObjectUpdate();

	UFUNCTION(BlueprintCallable)
	void PerformOnClick();

	UFUNCTION(BlueprintCallable)
	void PerformOnHover(bool bInActive);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	/**
	 * Auto capture the widget from the given button
	 * Button will serve to focus widget and some another stuff
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> DefaultButtonSlot;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default")
	FSSWidgetSlotEntryEvent OnClickEvent;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default")
	FSSWidgetSlotEntryBoolEvent OnHoverEvent;
	
};

