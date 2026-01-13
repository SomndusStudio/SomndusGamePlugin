// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SSGameActivatableWidget.h"
#include "NativeGameplayTags.h"
#include "UI/SSGameMessagingSubsystem.h"
#include "SSGameHUDLayout.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_ACTION_ESCAPE);

// For focus / visible manager
namespace SSGameplayTags
{
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_GamePlayer);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_GameOverlay);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_ModeMenu);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_Dialogue);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_Cinematic);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_CommonScreen);
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_UI_Layer_Modal);
}

/**
 * @class USSGameHUDLayout
 * @brief Defines the base layout for the game's Heads-Up Display (HUD).
 *
 * This class provides base functionality for managing and interacting with
 * the game's HUD. It is an abstract, blueprintable class that extends
 * the functionality of USSGameActivatableWidget.
 *
 * Features include support for escape menu management and defining the behavior
 * for handling escape actions within the game.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Game HUD Layout", Category = "Somndus Game|HUD"))
class SOMNDUSGAME_API USSGameHUDLayout : public USSGameActivatableWidget
{
	GENERATED_BODY()
	
	
protected:

	bool TryDeactivateLoadingWidget(FGameplayTag LoadingType);
	
	UFUNCTION()
	void OnLoadingTokenChange(USSGameMessagingSubsystem* GameMessagingSubsystem, FGameplayTag LoadingType, bool bRemoved);

	void HandleGlobalLoadingResult(ECommonMessagingResult CommonMessagingResult);
	
	static UCommonGameDialogDescriptor* CreateLoadingDescriptor(const FText& Body);
	
	void NativeOnInitialized() override;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanPerformEscapeAction();

protected:

	UPROPERTY()
	TMap<FGameplayTag, UCommonActivatableWidget*> LoadingWidgets;

	FCommonMessagingResultDelegate GlobalLoadingResultCallback;
	
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSoftClassPtr<UCommonActivatableWidget>> LoadingWidgetClasses;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;

};
