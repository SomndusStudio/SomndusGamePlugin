/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonUIFunctionLibrary.h"

#include "CommonUIVisibilitySubsystem.h"
#include "SSLog.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/ScrollBox.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SSGameUIManagerSubsystem.h"
#include "UI/SSWidgetObjectEntry.h"
#include "UI/Notification/SSGameNotificationManager.h"
#include "UObject/UObjectIterator.h"

USSGameNotificationManager* USSCommonUIFunctionLibrary::GetNotificationManager(UObject* WorldContextObject)
{
	return USSGameNotificationManager::Get(WorldContextObject);
}

void USSCommonUIFunctionLibrary::SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject)
{
	if (ISSWidgetObjectEntry* NativeImplementation = Cast<ISSWidgetObjectEntry>(EntryWidget))
	{
		NativeImplementation->NativeOnItemObjectSet(InItemObject);
	}
	else if (EntryWidget->Implements<USSWidgetObjectEntry>())
	{
		ISSWidgetObjectEntry::Execute_OnItemObjectSet(EntryWidget, InItemObject);
	}
}

ULocalPlayer* USSCommonUIFunctionLibrary::GetLocalPlayerFromContext(const UObject* WorldContextObject)
{
	ULocalPlayer* LocalPlayer = nullptr;
	if (WorldContextObject)
	{
		if (const UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
		{
			LocalPlayer = UserWidget->GetOwningLocalPlayer<ULocalPlayer>();
		}
		else if (const APlayerController* PC = Cast<APlayerController>(WorldContextObject))
		{
			LocalPlayer = PC->GetLocalPlayer();
		}
		else if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>())
			{
				if (const auto* PrimaryPC = GameInstance->GetPrimaryPlayerController(false))
				{
					LocalPlayer = PrimaryPC->GetLocalPlayer();
				}
			}
		}
	}
	return LocalPlayer;
}

APlayerController* USSCommonUIFunctionLibrary::GetLocalPlayerControllerFromContext(UObject* WorldContextObject)
{
	APlayerController* LocalPlayerController = nullptr;
	if (WorldContextObject)
	{
		if (UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
		{
			LocalPlayerController = UserWidget->GetOwningLocalPlayer<ULocalPlayer>()->PlayerController;
		}
		else if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
		{
			LocalPlayerController = PC;
		}
		else if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>())
			{
				LocalPlayerController = GameInstance->GetPrimaryPlayerController(false);
			}
		}
	}
	return LocalPlayerController;
}

FVector2D USSCommonUIFunctionLibrary::GetWidgetInvokerPosition(UUserWidget* Widget, UUserWidget* Invoker)
{
	// Get desired position
	FGeometry WidgetGeometry = Invoker->GetTickSpaceGeometry();
	WidgetGeometry.GetLocalSize();

	// Convertir la position locale en coordonnées écran
	FVector2D AbsolutePosition;
	FVector2D PixelPosition;
	FVector2D LocalPosition(0, 0); // On prend l'origine du widget


	auto* PlayerController = UGameplayStatics::GetPlayerController(Invoker, 0);
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, FVector(0, 0, 0), AbsolutePosition, false);

	USlateBlueprintLibrary::LocalToViewport(Invoker, WidgetGeometry, LocalPosition, PixelPosition, AbsolutePosition);

	FVector2D WidgetPosition = AbsolutePosition;

	// Size offset
	WidgetPosition.X += WidgetGeometry.GetLocalSize().X;

	return WidgetPosition;
}

FVector2D USSCommonUIFunctionLibrary::HardPositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker, const FVector2D& WidgetPosition)
{
	// Add to viewport if necessary
	if (!Widget->IsInViewport())
	{
		Widget->AddToViewport();
	}
	// Visibility
	Widget->SetVisibility(ESlateVisibility::HitTestInvisible);

	// Set position
	Widget->SetPositionInViewport(WidgetPosition, false);

	return WidgetPosition;
}

FVector2D USSCommonUIFunctionLibrary::PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker)
{
	const FVector2D WidgetPosition = GetWidgetInvokerPosition(Widget, Invoker);

	return HardPositionWidgetToInvoker(Widget, Invoker, WidgetPosition);
}

USSTooltipWidgetBase* USSCommonUIFunctionLibrary::ShowTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass,
                                                              UUserWidget* Invoker, UObject* DataObject, bool Active)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->TooltipManager->ShowTooltip(TooltipClass, Invoker, DataObject, Active);
	}
	return nullptr;
}

void USSCommonUIFunctionLibrary::HideTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->TooltipManager->RemoveTooltipPool(TooltipClass, Invoker);
	}
}

void USSCommonUIFunctionLibrary::HideCurrentTooltip(UObject* WorldContextObject)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->TooltipManager->HideCurrentTooltip();
	}
}

void USSCommonUIFunctionLibrary::ShowCursor(UObject* WorldContextObject, UUserWidget* Invoker)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->CursorManager->ShowCursor(Invoker);
	}
}

void USSCommonUIFunctionLibrary::HideCursor(UObject* WorldContextObject, UUserWidget* Invoker)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->CursorManager->HideCursor(Invoker);
	}
}

void USSCommonUIFunctionLibrary::StoreCacheFocusedWidget(UObject* WorldContextObject, UWidget* Widget)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		UISubsystem->CacheFocusedWidget = Widget;
	}
}

void USSCommonUIFunctionLibrary::ClearCacheFocusedWidget(UObject* WorldContextObject, UWidget* Widget)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		if (UISubsystem->CacheFocusedWidget == Widget)
		{
			UISubsystem->CacheFocusedWidget = nullptr;
		}
	}
}

FVector2D USSCommonUIFunctionLibrary::GetAbsolutePosition(UUserWidget* UserWidget)
{
	FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetAbsolutePosition();
}

void USSCommonUIFunctionLibrary::AbsoluteToViewport(UUserWidget* UserWidget, FVector2D AbsoluteDesktopCoordinate,
                                                    FVector2D& PixelPosition, FVector2D& ViewportPosition)
{
	USlateBlueprintLibrary::AbsoluteToViewport(UserWidget, AbsoluteDesktopCoordinate, PixelPosition, ViewportPosition);
}

UWidgetAnimation* USSCommonUIFunctionLibrary::TryGetWidgetAnimation(UUserWidget* UserWidget, FName InAnimationName)
{
	UWidgetBlueprintGeneratedClass* WidgetClass = UserWidget->GetWidgetTreeOwningClass();

	// Try to play reject animation dynamically 
	for (int i = 0; i < WidgetClass->Animations.Num(); i++)
	{
		auto Animation = WidgetClass->Animations[i];
		FString AnimName = Animation->GetName();
		FString CompareName = FString::Format(TEXT("{0}_INST"), {InAnimationName.ToString()});
		if (AnimName == CompareName)
		{
			return Animation;
		}
	}
	return nullptr;
}

FVector2D USSCommonUIFunctionLibrary::GetTopLeftPosition(UUserWidget* UserWidget)
{
	/*
	if (!UserWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget is not in viewport."));
		return FVector2D();
	}
	*/

	FGeometry Geometry = UserWidget->GetCachedGeometry();

	FVector2D AbsoluteTopLeftPosition = Geometry.GetAbsolutePosition();

	// Convert the absolute position to screen space using USlateBlueprintLibrary
	FVector2D ScreenPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(UserWidget, AbsoluteTopLeftPosition, ScreenPosition, ViewportPosition);

	return ScreenPosition;
}

USSSettingDataObject* USSCommonUIFunctionLibrary::BP_TryResolveData(USSSettingDataObject* Target, FName InIdentifier, TSubclassOf<USSSettingDataObject> DataClass,
                                                                    bool& bValid)
{
	bValid = false;
	if (!Target)
	{
		return nullptr;
	}

	if (Target->IsA(DataClass) && Target->Identifier == InIdentifier)
	{
		bValid = true;
		return Target;
	}

	return nullptr;
}

void USSCommonUIFunctionLibrary::CommonModalNavigationRules(UWidget* Widget)
{
	// Lock navigation from modal root
	if (Widget)
	{
		Widget->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
		Widget->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
		Widget->SetNavigationRuleBase(EUINavigation::Left, EUINavigationRule::Stop);
		Widget->SetNavigationRuleBase(EUINavigation::Right, EUINavigationRule::Stop);
	}
}

bool USSCommonUIFunctionLibrary::IsScrollBoxScrollable(UScrollBox* ScrollBox)
{
	if (!ScrollBox) return false;

	float ContentHeight = ScrollBox->GetScrollOffsetOfEnd();
	float ViewportHeight = ScrollBox->GetCachedGeometry().GetLocalSize().Y;

	if (ViewportHeight <= 0.0f) return false;

	return ContentHeight > 0.f;
}

bool USSCommonUIFunctionLibrary::CheckPlatformTraitActive(UObject* WorldContextObject, const FGameplayTag VisibilityTag)
{
	// Retrieve the local player controller from the context object
	auto* PlayerController = GetLocalPlayerControllerFromContext(WorldContextObject);

	// Ensure the subsystem is valid and check if the visibility tag is active
	const bool bHasTag = UCommonUIVisibilitySubsystem::GetChecked(PlayerController->GetLocalPlayer())->HasVisibilityTag(VisibilityTag);

	UE_LOG(LogSomndusGame, Verbose, TEXT("CheckPlatformTraitActive: Tag [%s] is %sactive."), *VisibilityTag.ToString(), bHasTag ? TEXT("") : TEXT("not "));

	return bHasTag;
}

bool USSCommonUIFunctionLibrary::CheckContainsPlatformTrait(UObject* WorldContextObject, const TArray<FGameplayTag>& VisibilityTags)
{
	// Iterate through each visibility tag and ensure all are active
	for (const auto& VisibilityTag : VisibilityTags)
	{
		if (!CheckPlatformTraitActive(WorldContextObject, VisibilityTag))
		{
			UE_LOG(LogSomndusGame, Verbose, TEXT("CheckContainsPlatformTrait: Tag [%s] is not active. Returning false."), *VisibilityTag.ToString());
			return false;
		}
	}
	return true;
}

void USSCommonUIFunctionLibrary::ClearChildrenExceptFirst(UPanelWidget* PanelWidget)
{
	for (int32 ChildIndex = PanelWidget->GetChildrenCount() - 1; ChildIndex >= 1; --ChildIndex)
	{
		PanelWidget->RemoveChildAt(ChildIndex);
	}
}

void USSCommonUIFunctionLibrary::CollapseAllChildren(UPanelWidget* PanelWidget)
{
	for (const auto& ChildWidget : PanelWidget->GetAllChildren())
	{
		ChildWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UWidget* USSCommonUIFunctionLibrary::GetOrCreateChildWidget(UPanelWidget* PanelWidget,
                                                            TSubclassOf<UUserWidget> WidgetClass, int32 Index)
{
	if (auto* ChildWidget = PanelWidget->GetChildAt(Index))
	{
		if (ChildWidget->IsA(WidgetClass))
		{
			return ChildWidget;
		}
	}

	auto* ChildWidget = CreateWidget(PanelWidget, WidgetClass);
	PanelWidget->AddChild(ChildWidget);

	return ChildWidget;
}

UWidget* USSCommonUIFunctionLibrary::GetOrCreateGridChildWidget(
	UGridPanel* GridPanel,
	TSubclassOf<UUserWidget> WidgetClass,
	int32 Index,
	int32 MaxColumn
)
{
	if (!GridPanel || !WidgetClass)
	{
		return nullptr;
	}

	if (GridPanel->GetChildrenCount() > Index)
	{
		UWidget* ExistingChild = GridPanel->GetChildAt(Index);
		if (ExistingChild && ExistingChild->IsA(WidgetClass))
		{
			return ExistingChild;
		}
	}

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GridPanel, WidgetClass);
	if (!NewWidget)
	{
		return nullptr;
	}

	// Calculate row and col
	int32 Row = Index / MaxColumn;
	int32 Column = Index % MaxColumn;

	// Add to grid panel
	UPanelSlot* PanelSlot = GridPanel->AddChild(NewWidget);
	if (UGridSlot* GridSlot = Cast<UGridSlot>(PanelSlot))
	{
		GridSlot->SetRow(Row);
		GridSlot->SetColumn(Column);
	}

	return NewWidget;
}

UWidget* USSCommonUIFunctionLibrary::GetFocusedWidget(UObject* WorldContextObject, int32 UserIndex)
{
	if (auto* UISubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameUIManagerSubsystem>())
	{
		return UISubsystem->CacheFocusedWidget;
	}
	return nullptr;
}

bool USSCommonUIFunctionLibrary::ScrollBox_MouseWheelScroll(const FPointerEvent& InMouseEvent, UScrollBox* ScrollBox, float Sensitivity)
{
	float ScrollValue = 0.f;

	const float Value = InMouseEvent.GetWheelDelta();
	
	ScrollValue = Value * Sensitivity;
	
	// Calculate the new scroll offset, subtracting the input value
	float NewScrollOffset = ScrollBox->GetScrollOffset() - ScrollValue;

	// Clamp the new offset between 0 and the max scrollable range
	NewScrollOffset = FMath::Clamp(NewScrollOffset, 0.f, ScrollBox->GetScrollOffsetOfEnd());
	
	// Apply the new scroll offset
	ScrollBox->SetScrollOffset(NewScrollOffset);

	return true;
}
