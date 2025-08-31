/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSSettingRegistryDataAsset.h"
#include "SSSettingWidgetThemeDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "SSSettingWidget_VList.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingWidget_VList : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bUsePreviewMode = false;

	UPROPERTY(EditAnywhere)
	bool bAutoInitialize = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USSSettingWidgetThemeDataAsset> SettingWidgetThemeDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USSSettingRegistryDataAsset> SettingRegistryDataAsset;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<USSSettingDataObject>> DataObjects;
	
	UFUNCTION(BlueprintCallable)
	void Setup(USSSettingWidgetThemeDataAsset* InSettingWidgetThemeDataAsset, USSSettingRegistryDataAsset* InSettingRegistryDataAsset);

	UFUNCTION(BlueprintCallable)
	void SetSettingRegistryDataAsset(USSSettingRegistryDataAsset* InSettingRegistryDataAsset, bool bRefresh=true);
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;

public:
	void PopulateList();

	UFUNCTION(BlueprintCallable)
	TArray<USSSettingDataObject*> GetDataObjects();

	UFUNCTION(BlueprintCallable)
	TArray<USSSettingWidgetObject*> GetWidgetObjects();

	UFUNCTION(BlueprintCallable)
	void PerformApply();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemListBox;
};
