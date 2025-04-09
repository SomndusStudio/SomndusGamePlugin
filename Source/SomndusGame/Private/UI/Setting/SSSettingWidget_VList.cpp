// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/SSSettingWidget_VList.h"

#include "Blueprint/WidgetTree.h"
#include "UI/Setting/SSSettingDataObject.h"
#include "UI/Setting/SSSettingWidgetEntry.h"

void USSSettingWidget_VList::Setup(USSSettingWidgetThemeDataAsset* InSettingWidgetThemeDataAsset, USSSettingRegistryDataAsset* InSettingRegistryDataAsset)
{
	SettingWidgetThemeDataAsset = InSettingWidgetThemeDataAsset;
	SettingRegistryDataAsset = InSettingRegistryDataAsset;
}

void USSSettingWidget_VList::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (bUsePreviewMode)
	{
		if (IsDesignTime())
		{
			PopulateList();
		}
	}
}

void USSSettingWidget_VList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (bAutoInitialize)
	{
		PopulateList();
	}
}

void USSSettingWidget_VList::PopulateList()
{
	if (!SettingWidgetThemeDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Not valid SettingWidgetThemeDataAsset"), *GetNameSafe(this));
		return;
	}

	if (!SettingRegistryDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Not valid SettingRegistryDataAsset"), *GetNameSafe(this));
		return;
	}

	if (!ItemListBox)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Not valid ItemListBox (VerticalBox). Have vertical box named ItemListBox"), *GetNameSafe(this));
		return;
	}

	// Clear existing widgets from the VerticalBox
	ItemListBox->ClearChildren();

	for (const auto& Setting : SettingRegistryDataAsset->Settings)
	{
		// Link world context object for runtime stuff
		if (Setting->Handler)
		{
			Setting->Handler->WorldObjectContext = this;
		}

		// Transfer identifier
		Setting->Data->Identifier = Setting->Identifier;
		
		// Store data object ref
		DataObjects.Add(Setting->Data);
		
		// Get theme
		auto& SettingWidgetTheme = SettingWidgetThemeDataAsset->GetSettingThemeInfo(Setting->TypeTag);
		
		// If theme not valid ignore this setting
		if (SettingWidgetTheme.SoftWidgetClass.IsNull())
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Unable to found valid SettingWidgetTheme from Tag -> %s"),  *GetNameSafe(this), *Setting->TypeTag.ToString());
			return;
		};

		// Load widget class
		auto WidgetClass = SettingWidgetTheme.SoftWidgetClass.Get();
		if (!SettingWidgetTheme.SoftWidgetClass.IsValid())
		{
			WidgetClass = SettingWidgetTheme.SoftWidgetClass.LoadSynchronous();
		}
		
		// Create a new widget instance of the specified class
		UUserWidget* NewItemWidget = CreateWidget<UUserWidget>(this, WidgetClass);

		if (!NewItemWidget) return;
		
		if (NewItemWidget->Implements<USSSettingWidgetEntry>())
		{
			// Set data on the new widget (e.g., item name)
			ISSSettingWidgetEntry::Execute_SetData(NewItemWidget, Setting->Data);
			// Add the widget to the VerticalBox
			ItemListBox->AddChildToVerticalBox(NewItemWidget);
		}
	}
}

TArray<USSSettingDataObject*> USSSettingWidget_VList::GetDataObjects()
{
	return DataObjects;
}

TArray<USSSettingWidgetObject*> USSSettingWidget_VList::GetWidgetObjects()
{
	return SettingRegistryDataAsset->Settings;
}

void USSSettingWidget_VList::PerformApply()
{
	for (const auto& SettingWidgetObject: SettingRegistryDataAsset->Settings)
	{
		if (!SettingWidgetObject->Handler) continue;

		SettingWidgetObject->Handler->NativeApply(SettingWidgetObject->Data);
		SettingWidgetObject->Data->NotifyDefaultValueChange();
	}
}
