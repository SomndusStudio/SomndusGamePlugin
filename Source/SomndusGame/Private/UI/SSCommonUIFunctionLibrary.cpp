/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonUIFunctionLibrary.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/ScrollBox.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SSGameUIManagerSubsystem.h"
#include "UI/SSWidgetObjectEntry.h"
#include "UI/Notification/SSGameNotificationManager.h"

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
