/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSTabListWidget.h"

TArray<UWidget*> USSTabListWidget::GetChildItems_Implementation()
{
	return TArray<UWidget*>();
}

void USSTabListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnTabSelected.AddUniqueDynamic(this, &USSTabListWidget::OnNativeTabSelected);
}

void USSTabListWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USSTabListWidget::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);

	StoredButtons.Add(TabNameID, TabButton);
}

void USSTabListWidget::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);

	StoredButtons.Remove(TabNameID);
}

void USSTabListWidget::NativeNotifyTabInfoSelected(FName TabId)
{
	if (const FInstancedStruct* Info = PendingTabInfoMap.Find(TabId))
	{
		OnTabInfoSelected.Broadcast(this, TabId, *Info);
	}
}

void USSTabListWidget::OnNativeTabSelected(FName TabId)
{
	// only while construction not processing
	if (!bIsDeferredProcess)
	{
		NativeNotifyTabInfoSelected(TabId);
	}
}

void USSTabListWidget::ForceCallTabSelected(FName TabId)
{
	SelectTabByID(TabId, true);
	NativeNotifyTabInfoSelected(TabId);
}

void USSTabListWidget::BP_NextAction()
{
	bool bPassthrough;
	HandleNextTabInputAction(bPassthrough);
}

void USSTabListWidget::BP_PreviousAction()
{
	bool bPassthrough;
	HandlePreviousTabInputAction(bPassthrough);
}