// ©2025 Schartier Isaac. All rights reserved.


#include "GameFeatures/GameFeatureAction_GameplayReady.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/SSGameModeManagerComponent.h"

class UDUGameModeManagerComponent;

void UGameFeatureAction_GameplayReady::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if ensure(ActiveData)
	{
		Reset(*ActiveData);
	}
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_GameplayReady::IsDataValid(class FDataValidationContext& Context) const
{
	return Super::IsDataValid(Context);
}
#endif

void UGameFeatureAction_GameplayReady::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		UE_LOG(LogTemp, Warning, TEXT("Extension removed"));
	}
	else
	{
		ActiveData.TagTokens.AddUnique(EventName);

		CheckGameplayIsReady(Actor, ActiveData);
	}
}

void UGameFeatureAction_GameplayReady::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{			
			// Listen Player Character Ready
			TSoftClassPtr<AActor> PLayerCharacterClass = ACharacter::StaticClass();
			
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				PLayerCharacterClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);

			// Listen player controller
			TSoftClassPtr<AActor> ControllerClass = APlayerController::StaticClass();
			
			TSharedPtr<FComponentRequestHandle> Controller_ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				ControllerClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(Controller_ExtensionRequestHandle);

			// Listen HUD
			TSoftClassPtr<AActor> HUDClass = AHUD::StaticClass();
			
			TSharedPtr<FComponentRequestHandle> HUD_ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(HUD_ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_GameplayReady::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.TagTokens.Empty();
	
	ActiveData.bGameplayReady = false;
}

bool UGameFeatureAction_GameplayReady::CheckGameplayIsReady(AActor* Actor, FPerContextData& ActiveData)
{
	int32 CountValidToken = 0;
	int32 RequiredTokenCount = TagTokens.Num();
	
	for (const auto& TagToken : ActiveData.TagTokens)
	{
		if (TagTokens.Contains(TagToken))
		{
			CountValidToken++;
		}
	}
	if (CountValidToken != RequiredTokenCount) return false;

	if (ActiveData.bGameplayReady) return false;

	ActiveData.bGameplayReady = true;
	NotifyGameplayIsReady(Actor);
	
	return true;
}

void UGameFeatureAction_GameplayReady::NotifyGameplayIsReady(AActor* Actor)
{
	auto* GameState = UGameplayStatics::GetGameState(Actor);
	auto* CapturedGameModeManagerComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();

	CapturedGameModeManagerComponent->SetGameplayReadyDelayed();
}
