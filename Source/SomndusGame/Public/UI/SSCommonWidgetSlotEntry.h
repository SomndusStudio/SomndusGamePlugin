// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "CommonTabListWidgetBase.h"
#include "SSCommonFocusWidget.h"
#include "SSWidgetObjectEntry.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "SSCommonWidgetSlotEntry.generated.h"

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
	void OnItemObjectUpdate();
	
	/**
	 * Auto capture the widget from the given button
	 * Button will serve to focus widget and some another stuff
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> DefaultButtonSlot;
};

