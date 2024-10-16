/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "SSCommonFocusWidget.h"
#include "SSWidgetObjectEntry.h"

#include "SSCommonWidgetSlotEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSWidgetSlotEntryEvent, class USSCommonWidgetSlotEntry*, SlotEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSWidgetSlotEntryBoolEvent, class USSCommonWidgetSlotEntry*, SlotEntry, bool, bActive);

/**
 * 
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
	UObject* GetItemObject() { return ItemObject; }
	
	virtual void OnItemObjectSet_Implementation(UObject* InItemObject) override;
	
	virtual const UWidget* GetDefaultFocusWidget_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreOnItemObjectUpdate();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnItemObjectUpdate();

	UFUNCTION(BlueprintCallable)
	void PerformOnClick();

	UFUNCTION(BlueprintCallable)
	void PerformOnHover(bool bActive);

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

