// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFramework/Character.h"
#include "GameFeatureAction_AddWidgets.generated.h"


struct FWorldContext;
struct FComponentRequestHandle;

USTRUCT()
struct FAddWidgetRequestRequirement
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> PlayerActorClass = ACharacter::StaticClass();
	
	UPROPERTY(EditAnywhere)
	bool bShouldPlayerGameplayReady = true;
	
	UPROPERTY(EditAnywhere)
	bool bShouldInputReady = true;
};

USTRUCT()
struct FHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category=UI, meta=(Categories="UI.Layer,Local.UI.Layer"))
	FGameplayTag LayerID;

	/**
	 * Used to know if widget can only be used in editor/debug mode
	 * No used in shipping production
	 */
	UPROPERTY(EditAnywhere, Category=UI)
	bool bOnlyDebugMode = false;
};

USTRUCT()
struct FHUDElementEntry
{
	GENERATED_BODY()

	// The widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	// The slot ID where we should place this widget
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;

	/**
	 * Used to know if widget can only be used in editor/debug mode
	 * No used in shipping production
	 */
	UPROPERTY(EditAnywhere, Category=UI)
	bool bOnlyDebugMode = false;
};


/**
 * GameFeatureAction responsible for granting abilities (and attributes) to actors of a specified type.
 */
UCLASS(meta = (DisplayName = "Add Widgets"))
class SOMNDUSGAMEMODULAR_API UGameFeatureAction_AddWidgets : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
	
public:
	
#if !UE_SERVER
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#endif
	
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface

private:
	// Requirement
	UPROPERTY(EditAnywhere, Category=UI)
	FAddWidgetRequestRequirement RequestRequirement;	
	
	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{LayerID} -> {LayoutClass}"))
	TArray<FHUDLayoutRequest> Layout;

	// Widgets to add to the HUD
	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{SlotID} -> {WidgetClass}"))
	TArray<FHUDElementEntry> Widgets;

private:

	struct FPerActorData
	{
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TMap<FObjectKey, FPerActorData> ActorData;
		bool bInputCacheLoaded = false;
		bool bHeroLoaded = false;
		bool bLoaded = false;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

#if !UE_SERVER
	void Reset(FPerContextData& ActiveData);

	bool CheckShouldAddWidgets(AActor* Actor, FPerContextData& ActiveData);
	
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	
	AHUD* GetHUD(AActor* Actor);
	
	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
#endif
};
