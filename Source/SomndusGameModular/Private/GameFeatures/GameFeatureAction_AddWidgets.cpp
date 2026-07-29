// ©2025 Schartier Isaac. All rights reserved.


#include "GameFeatures/GameFeatureAction_AddWidgets.h"

#include "CommonPlayerController.h"
#include "CommonUIExtensions.h"
#include "GameFeaturesSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "SSLog.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "Helper/SSHelperStatics.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "CommonSomndusGame"

static TAutoConsoleVariable<bool> ShowDebugWidgetEntry(TEXT("Somndus.Debug.WidgetEntry"), false, TEXT("Show debug widget entry ?"));

#if !UE_SERVER
void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* ActiveData = ContextData.Find(Context);
	if ensure(ActiveData)
	{
		Reset(*ActiveData);
	}
}
#endif

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddWidgets::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	for (const FHUDElementEntry& Entry : Widgets)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.WidgetClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddWidgets::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	{
		int32 EntryIndex = 0;
		for (const FHUDLayoutRequest& Entry : Layout)
		{
			if (Entry.LayoutClass.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("LayoutHasNullClass", "Null WidgetClass at index {0} in Layout"), FText::AsNumber(EntryIndex)));
			}

			if (!Entry.LayerID.IsValid())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("LayoutHasNoTag", "LayerID is not set at index {0} in Widgets"), FText::AsNumber(EntryIndex)));
			}

			++EntryIndex;
		}
	}

	{
		int32 EntryIndex = 0;
		for (const FHUDElementEntry& Entry : Widgets)
		{
			if (Entry.WidgetClass.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullClass", "Null WidgetClass at index {0} in Widgets"), FText::AsNumber(EntryIndex)));
			}

			if (!Entry.SlotID.IsValid())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNoTag", "SlotID is not set at index {0} in Widgets"), FText::AsNumber(EntryIndex)));
			}
			++EntryIndex;
		}
	}

	return Result;
}
#endif

void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
#if !UE_SERVER
	UWorld* World = WorldContext.World();
	
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{			
			// Listen Player Character Ready
			TSoftClassPtr<AActor> PlayerActorClass = RequestRequirement.PlayerActorClass;
			
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				PlayerActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);

			// Listen player controller
			TSoftClassPtr<AActor> ControllerClass = APlayerController::StaticClass();
			
			TSharedPtr<FComponentRequestHandle> Controller_ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				ControllerClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(Controller_ExtensionRequestHandle);
		}
	}
#endif
}

#if !UE_SERVER
void UGameFeatureAction_AddWidgets::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();

	ActiveData.bInputCacheLoaded = false;
	ActiveData.bHeroLoaded = false;
	ActiveData.bLoaded = false;
	
	for (TPair<FObjectKey, FPerActorData>& Pair : ActiveData.ActorData)
	{
		for (FUIExtensionHandle& Handle : Pair.Value.ExtensionHandles)
		{
			Handle.Unregister();
		}
	}
	ActiveData.ActorData.Empty();
}

bool UGameFeatureAction_AddWidgets::CheckShouldAddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	// If input system loaded (for gamepad/console icons)
	if (RequestRequirement.bShouldPlayerGameplayReady)
	{
		if (!ActiveData.bInputCacheLoaded)
		{
			return false;
		}
	}
	
	// If hero ready
	if (RequestRequirement.bShouldPlayerGameplayReady)
	{
		if (!ActiveData.bHeroLoaded)
		{
			return false;
		}
	}

	// Avoid duplicate entry
	if (ActiveData.bLoaded)
	{
		return false;
	}
	
	UE_LOG(LogSomndusGame, Log, TEXT("%s : Feature_AddWidgets"), *GetName());
			
	AddWidgets(Actor, ActiveData);

	return true;
}

void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	
	// Should be a controlled character player
	auto* Character = Cast<ACharacter>(Actor);
	if (!Character) return;
	
	if (!Character->IsPlayerControlled())
	{
		return;
	}

	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else
	{
		// If player hero ready
		if (EventName == "GameplayCharacterComponentReady")
		{
			ActiveData.bHeroLoaded=true;
		}

		if (EventName == "InputCacheLoaded")
		{
			ActiveData.bInputCacheLoaded=true;
		}
		
		CheckShouldAddWidgets(Actor, ActiveData);
	}
}

AHUD* UGameFeatureAction_AddWidgets::GetHUD(AActor* Actor)
{
	AHUD* HUD = nullptr;
	if (auto* PlayerController = Cast<APlayerController>(Actor))
	{
		HUD = PlayerController->GetHUD();
	}
	else
	{
		// Todo : Sometime it's null when exist game
		if (auto* LocalPlayerController = UGameplayStatics::GetPlayerController(Actor, 0))
		{
			HUD = LocalPlayerController->GetHUD();
		}
	}
	return HUD;
}

void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ActiveData.bLoaded = true;
	
	AHUD* HUD = GetHUD(Actor);
	
	if (!HUD->GetOwningPlayerController())
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		FPerActorData& ActorData = ActiveData.ActorData.FindOrAdd(HUD);

		for (const FHUDLayoutRequest& Entry : Layout)
		{
#if UE_BUILD_SHIPPING || UE_BUILD_SHIPPING_WITH_EDITOR
			if (Entry.bOnlyDebugMode)
			{
				continue;
			}
#else
			// Don't show the widget if only debug mode and debug mode deactivate
			if (Entry.bOnlyDebugMode)
			{
				if (!ShowDebugWidgetEntry.GetValueOnGameThread())
				{
					continue;
				}
			}
#endif
			auto ConcreteWidgetClass = USSHelperStatics::TryGetClass(Entry.LayoutClass);
			// ignore widget entry if it's for dev/debug and we are in shipping/production
			if (ConcreteWidgetClass)
			{
				ActorData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FHUDElementEntry& Entry : Widgets)
		{
#if UE_BUILD_SHIPPING || UE_BUILD_SHIPPING_WITH_EDITOR
			if (Entry.bOnlyDebugMode)
			{
				continue;
			}
#else
			// Don't show the widget if only debug mode and debug mode deactivate
			if (Entry.bOnlyDebugMode)
			{
				if (!ShowDebugWidgetEntry.GetValueOnGameThread())
				{
					continue;
				}
			}
#endif
			auto EntryWidgetClass = USSHelperStatics::TryGetClass(Entry.WidgetClass);
			ActorData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, EntryWidgetClass, -1));
		}
	}

	// Notify in HUD
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(HUD, "WidgetsAdded");
}

void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AHUD* HUD = GetHUD(Actor);

	if (!HUD)
	{
		return;
	}
	
	// Only unregister if this is the same HUD actor that was registered, there can be multiple active at once on the client
	FPerActorData* ActorData = ActiveData.ActorData.Find(HUD);

	if (ActorData)
	{
		for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActorData->LayoutsAdded)
		{
			if (AddedLayout.IsValid())
			{
				AddedLayout->DeactivateWidget();
			}
		}

		for (FUIExtensionHandle& Handle : ActorData->ExtensionHandles)
		{
			Handle.Unregister();
		}
		ActiveData.ActorData.Remove(HUD);
	}
}
#endif

#undef LOCTEXT_NAMESPACE
