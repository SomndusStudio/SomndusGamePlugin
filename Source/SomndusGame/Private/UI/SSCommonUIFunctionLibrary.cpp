/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonUIFunctionLibrary.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SSGameUIManagerSubsystem.h"
#include "UI/SSWidgetObjectEntry.h"

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
