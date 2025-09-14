/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Tooltip/SSTooltipManager.h"

#include "TimerManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/SSCommonUIFunctionLibrary.h"
#include "UI/Tooltip/SSTooltipInvoker.h"
#include "UI/Tooltip/SSTooltipWidgetBase.h"


bool USSTooltipManager::Tick(float DeltaSeconds)
{
	if (IsValid(CurrentTooltip))
	{
		if (!TryFixTooltip(this, TooltipInvoker, CurrentTooltip))
		{
			FVector2D InvokerPosition = GetWidgetInvokerPosition(CurrentTooltip, TooltipInvoker);
			
			// If invoker position changed
			if (LastTooltipInvokerPosition != InvokerPosition)
			{
				// Refresh position
				LastTooltipInvokerPosition = USSCommonUIFunctionLibrary::HardPositionWidgetToInvoker(CurrentTooltip, TooltipInvoker, InvokerPosition);
			}
		}
	}
	return true;
}

void USSTooltipManager::AddTooltip(USSTooltipWidgetBase* TooltipWidget)
{
	DistinctTooltips.AddUnique(TooltipWidget);
}

void USSTooltipManager::ClearTooltip()
{
	if (IsValid(CurrentTooltip))
	{
		RemoveTooltipPool(CurrentTooltip->GetClass(), TooltipInvoker, true);
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

bool USSTooltipManager::TryFixTooltip(UObject* WorldContextObject, UUserWidget* InvokerWidget, UUserWidget* TooltipWidget)
{
	bool bShouldFix = false;
	// Get desired position
	FVector2D NewPosition = TooltipWidget->GetCachedGeometry().GetAbsolutePosition();
	FVector2D PixelPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(WorldContextObject,TooltipWidget->GetCachedGeometry().GetAbsolutePosition(), PixelPosition, NewPosition);
	
	// Widget Size
	FVector2D WidgetSize;
	FVector2D PixelPositionSize;
	USlateBlueprintLibrary::AbsoluteToViewport(WorldContextObject, TooltipWidget->GetCachedGeometry().GetAbsoluteSize(), PixelPositionSize, WidgetSize);
	
	// Screen size
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	
	// Check if widget does not exceed the X screen
	if (WidgetSize.X + NewPosition.X > ViewportSize.X)
	{
		// should position in left to invoker
		FVector2D InvokerPixel;
		FVector2D InvokerViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(WorldContextObject, InvokerWidget->GetCachedGeometry().GetAbsolutePosition(), InvokerPixel, InvokerViewportPosition);
		NewPosition.X = InvokerViewportPosition.X - WidgetSize.X;
		bShouldFix = true;
	}
	// Check if widget does not exceed the Y screen
	if (WidgetSize.Y + NewPosition.Y > ViewportSize.Y)
	{
		NewPosition.Y = ViewportSize.Y - WidgetSize.Y;
		bShouldFix = true;
	}
	// Re position to viewport
	if (bShouldFix)
	{
		TooltipWidget->SetPositionInViewport(NewPosition, false);
	}

	return bShouldFix;
}

FVector2D USSTooltipManager::GetWidgetInvokerPosition(UUserWidget* Widget, UUserWidget* Invoker)
{
	FVector2D WidgetPosition = USSCommonUIFunctionLibrary::GetWidgetInvokerPosition(Widget, Invoker);

	// position offset
	if (Invoker->Implements<USSTooltipInvoker>())
	{
		WidgetPosition += ISSTooltipInvoker::Execute_GetTooltipOffsetPosition(Invoker, Widget);
	}

	return WidgetPosition;
}

FVector2D USSTooltipManager::PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker)
{
	FVector2D WidgetPosition = GetWidgetInvokerPosition(Widget, Invoker);

	return USSCommonUIFunctionLibrary::HardPositionWidgetToInvoker(Widget, Invoker, WidgetPosition);
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

void USSTooltipManager::RemoveTooltipPool(const TSubclassOf<USSTooltipWidgetBase>& Class, UUserWidget* Invoker, bool bForceCurrentTick)
{
	TooltipPools.RemoveAll([Invoker](const FSSTooltipPool& Value) {
		return Value.WidgetToken == Invoker;
	});
	if (bForceCurrentTick)
	{
		OnDelayCheckPool();
	}
	else
	{
		// If tooltips widget class no exist
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USSTooltipManager::OnDelayCheckPool, 0.1f, false);
	}
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
			LastTooltipInvokerPosition = PositionWidgetToInvoker(TooltipWidget, Invoker);
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
	RemoveTooltipPool(CurrentTooltip->GetClass(), TooltipInvoker);
	//ShowTooltipWidget(CurrentTooltip, TooltipInvoker, nullptr, false);
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
