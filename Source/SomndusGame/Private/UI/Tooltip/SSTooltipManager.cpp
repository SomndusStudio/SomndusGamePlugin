/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Tooltip/SSTooltipManager.h"

#include "Blueprint/UserWidget.h"
#include "UI/Tooltip/SSTooltipInvoker.h"
#include "UI/Tooltip/SSTooltipWidgetBase.h"


void USSTooltipManager::AddTooltip(USSTooltipWidgetBase* TooltipWidget)
{
	DistinctTooltips.AddUnique(TooltipWidget);
}

void USSTooltipManager::ClearTooltip()
{
	if (IsValid(CurrentTooltip))
	{
		CurrentTooltip->RemoveFromParent();
	}
}

USSTooltipWidgetBase* USSTooltipManager::CheckTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass)
{
	// If not exist
	if (USSTooltipWidgetBase* Widget = GetTooltipByClass(TooltipClass))
	{
		return Widget;
	}
	// Create
	USSTooltipWidgetBase* Widget = CreateWidget<USSTooltipWidgetBase>(this->GetGameInstance(), TooltipClass);
	AddTooltip(Widget);

	return Widget;
}

FVector2D USSTooltipManager::GetWidgetPositionInViewport(UUserWidget* Widget)
{
	if (!Widget) return FVector2D::ZeroVector;

	const TSharedPtr<SWidget> SlateWidget = Widget->GetCachedWidget();
	if (SlateWidget.IsValid())
	{
		// Obtenir le Geometry
		const FGeometry Geometry = SlateWidget->GetCachedGeometry();

		// Récupérer la position locale à l'écran
		return Geometry.GetAbsolutePosition();
	}

	return FVector2D::ZeroVector;
}

void USSTooltipManager::PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker)
{
	// Add to viewport if necessary
	if (!Widget->IsInViewport())
	{
		Widget->AddToViewport();
	}
	// Visibility
	Widget->SetVisibility(ESlateVisibility::HitTestInvisible);

	// Get desired position
	// FVector2D PixelPosition;
	FVector2D ViewportPosition = GetWidgetPositionInViewport(Invoker);
	// USlateBlueprintLibrary::LocalToViewport(WorldContextObject, Invoker->GetCachedGeometry(), FVector2D(), PixelPosition, ViewportPosition);

	// position offset
	if (Invoker->Implements<USSTooltipInvoker>())
	{
		ViewportPosition += ISSTooltipInvoker::Execute_GetTooltipOffsetPosition(Invoker, Widget);
	}

	// Set position
	Widget->SetPositionInViewport(ViewportPosition, true);
}

void USSTooltipManager::OnDelayCheckPool()
{
	auto Tooltips = ActiveTooltips;
	for (auto* TooltipWidget : Tooltips)
	{
		bool bStillOne = false;
		for (const auto& Pool : TooltipPools)
		{
			if (Pool.TooltipClass == TooltipWidget->GetClass()){
				bStillOne = true;
				break;
			}
		}
		if (!bStillOne)
		{
			RemoveTooltipWidget(TooltipWidget);
		}
	}
}

void USSTooltipManager::RemoveTooltipWidget(USSTooltipWidgetBase* TooltipWidget)
{
	ActiveTooltips.Remove(TooltipWidget);
	TooltipWidget->UpdateDataObject(nullptr);
	if (CurrentTooltip == TooltipWidget)
	{
		TooltipInvoker = nullptr;
		CurrentTooltip = nullptr;
	}
	TooltipWidget->RemoveFromParent();
}

void USSTooltipManager::RemoveTooltipPool(const TSubclassOf<USSTooltipWidgetBase>& Class, UUserWidget* Invoker)
{
	TooltipPools.RemoveAll([Invoker](const FSSTooltipPool& Value) {
		return Value.WidgetToken == Invoker;
	});
	// If tooltips widget class no exist
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USSTooltipManager::OnDelayCheckPool, 0.1f, false);
}

void USSTooltipManager::ShowTooltipWidget(USSTooltipWidgetBase* TooltipWidget, UUserWidget* Invoker,
                                          UObject* DataObject, bool Active)
{
	if (IsValid(TooltipWidget))
	{
		// If should show
		if (Active)
		{
			// Update current tooltip
			TooltipInvoker = Invoker;
			CurrentTooltip = TooltipWidget;
			ActiveTooltips.AddUnique(CurrentTooltip);

			CurrentTooltip->UpdateDataObject(DataObject);
			
			// Position
			PositionWidgetToInvoker(TooltipWidget, Invoker);
		}
		// If hide
		else
		{
			RemoveTooltipWidget(TooltipWidget);
		}
	}
}

USSTooltipWidgetBase* USSTooltipManager::ShowTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass,
                                            UUserWidget* Invoker, UObject* DataObject, bool Active)
{
	// Pool
	FSSTooltipPool TooltipPool;
	TooltipPool.WidgetToken = Invoker;
	TooltipPool.TooltipClass = TooltipClass;

	if (Active)
	{
		TooltipPools.Add(TooltipPool);
	}
	// Check
	if (USSTooltipWidgetBase* Widget = CheckTooltip(TooltipClass))
	{
		ShowTooltipWidget(Widget, Invoker, DataObject, Active);
		return Widget;
	}
	return nullptr;
}

void USSTooltipManager::HideCurrentTooltip()
{
	ShowTooltipWidget(CurrentTooltip, TooltipInvoker, nullptr, false);
}

USSTooltipWidgetBase* USSTooltipManager::GetTooltipByClass(TSubclassOf<USSTooltipWidgetBase> TooltipClass)
{
	USSTooltipWidgetBase* Found = nullptr;
	for (auto* TooltipWidget : DistinctTooltips)
	{
		if (TooltipWidget && TooltipWidget->IsA(TooltipClass))
		{
			Found = TooltipWidget;
			break;
		}
	}
	return Found;
}
