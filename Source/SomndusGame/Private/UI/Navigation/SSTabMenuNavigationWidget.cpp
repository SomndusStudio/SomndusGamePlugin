/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Navigation/SSTabMenuNavigationWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"

void USSTabMenuNavigationWidget::SetupFromMenuEntries()
{
	// Clear
	PendingTabInfoMap.Empty();

	RemoveAllTabs();

	for (const auto& Entry : MenuEntries)
	{
		PendingTabInfoMap.Add(Entry.Value, FInstancedStruct::Make(Entry));

		RegisterTab(Entry.Value, TabButtonClass, nullptr, -1);
	}
}

void USSTabMenuNavigationWidget::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);

	if (!EntryParentWidget) return;

	if (UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(EntryParentWidget))
	{
		auto* HSlot = HorizontalBox->AddChildToHorizontalBox(TabButton);
		HSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		HSlot->SetHorizontalAlignment(HAlign_Fill);
	}

	if (UVerticalBox* VerticalBox = Cast<UVerticalBox>(EntryParentWidget))
	{
		VerticalBox->AddChildToVerticalBox(TabButton);
	}

	if (auto* TabInfo = PendingTabInfoMap.Find(TabNameID))
	{
		FSSMenuInfo MenuInfo = TabInfo->Get<FSSMenuInfo>();

		if (auto* TabMenuButton = Cast<USSTabButton>(TabButton))
		{
			TabMenuButton->UpdateMenuInfo(MenuInfo);
		}
	}
}

void USSTabMenuNavigationWidget::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);

	if (!EntryParentWidget) return;

	EntryParentWidget->RemoveChild(TabButton);
}

void USSTabMenuNavigationWidget::NativeNotifyTabInfoSelected(FName TabId)
{
	Super::NativeNotifyTabInfoSelected(TabId);
	// retrieve index
	if (auto* Button = StoredButtons.Find(TabId))
	{
		if (EntryParentWidget)
		{
			int32 ButtonChildIndex = EntryParentWidget->GetChildIndex(*Button);
			OnChange.Broadcast(this, ButtonChildIndex, TabId);
		}
	}
}

void USSTabMenuNavigationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PreviousTabActionButton)
	{
		PreviousTabActionButton->OnClicked().AddUObject(this, &ThisClass::HandlePreviousAction);
	}

	if (NextTabActionButton)
	{
		NextTabActionButton->OnClicked().AddUObject(this, &ThisClass::HandleNextAction);
	}
}


void USSTabMenuNavigationWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (PreviousTabActionButton)
	{
		PreviousTabActionButton->SetTriggeringInputAction(PreviousTriggerAction);
	}

	if (NextTabActionButton)
	{
		NextTabActionButton->SetTriggeringInputAction(NextTriggerAction);
	}

	// if should preview
	if (bPreview)
	{
		SetupFromMenuEntries();
	}
}


void USSTabMenuNavigationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetupFromMenuEntries();
}

void USSTabMenuNavigationWidget::HandlePreviousAction()
{
	BP_PreviousAction();
}

void USSTabMenuNavigationWidget::HandleNextAction()
{
	BP_NextAction();
}
