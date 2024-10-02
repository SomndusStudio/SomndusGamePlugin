// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "InstancedStruct.h"
#include "SSTabListWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSSNavigationEventSignature, class USSTabListWidget*, Widget, int32, Index, FName, Info);

/** Delegate broadcast when a new tab is selected. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSSOnTabInfoSelected, class USSTabListWidget*, Widget, FName, TabId, const FInstancedStruct, InstancedStruct);

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDeferredProcess = false;
	
	/**
	 * Stores label info for tabs that have been registered at runtime but not yet created. 
	 * Elements are removed once they are created.
	 */
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FInstancedStruct> PendingTabInfoMap;

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, UCommonButtonBase*> StoredButtons;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EventDispatchers")
	FSSNavigationEventSignature OnChange;

	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

	virtual void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<UWidget*> GetChildItems();
	
	UFUNCTION()
	void OnNativeTabSelected(FName TabId);

	UFUNCTION(BlueprintCallable)
	void ForceCallTabSelected(FName TabId);

	UFUNCTION(BlueprintCallable)
	void BP_NextAction();

	UFUNCTION(BlueprintCallable)
	void BP_PreviousAction();
	
	/** Broadcasts when a new tab is selected. */
	UPROPERTY(BlueprintAssignable, Category = TabList)
	FSSOnTabInfoSelected OnTabInfoSelected;
};
