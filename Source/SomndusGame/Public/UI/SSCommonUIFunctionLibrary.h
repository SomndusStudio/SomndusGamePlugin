/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Setting/SSSettingDataObject.h"
#include "Tooltip/SSTooltipWidgetBase.h"
#include "SSCommonUIFunctionLibrary.generated.h"

class USSGameNotificationManager;
class UScrollBox;
/**
 * A utility function library for common UI-related operations in the SOMNDUS game.
 * Provides functionalities such as tooltip management, cursor control, widget manipulation,
 * and other UI utilities used across the user interface system.
 */
UCLASS()
class SOMNDUSGAME_API USSCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns the game notification manager for the current local player.
	 */
	UFUNCTION(BlueprintCallable, Category = "SomndusGame|Notification", meta = (WorldContext = "WorldContextObject", CompactNodeTitle = "Notification Mgr"))
	static USSGameNotificationManager* GetNotificationManager(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="SS|UI")
	static void SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|UI", meta = (WorldContext = "WorldContextObject"))
	static ULocalPlayer* GetLocalPlayerFromContext(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|UI", meta = (WorldContext = "WorldContextObject"))
	static APlayerController* GetLocalPlayerControllerFromContext(UObject* WorldContextObject);

	static FVector2D GetWidgetInvokerPosition(UUserWidget* Widget, UUserWidget* Invoker);
	
	static FVector2D HardPositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker, const FVector2D& WidgetPosition);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static FVector2D PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	static USSTooltipWidgetBase* ShowTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker, UObject* DataObject, bool Active=true);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void HideTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void HideCurrentTooltip(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void ShowCursor(UObject* WorldContextObject, UUserWidget* Invoker);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void HideCursor(UObject* WorldContextObject, UUserWidget* Invoker);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetAbsolutePosition(UUserWidget* UserWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void AbsoluteToViewport(UUserWidget* UserWidget, FVector2D AbsoluteDesktopCoordinate, FVector2D& PixelPosition, FVector2D& ViewportPosition);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static UWidgetAnimation* TryGetWidgetAnimation(UUserWidget* UserWidget, FName InAnimationName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetTopLeftPosition(UUserWidget* UserWidget);

	UFUNCTION(BlueprintCallable, Category="SomndusGame", DisplayName="TryResolveData", meta=(DeterminesOutputType = "DataClass", ExpandBoolAsExecs="bValid"))
	static USSSettingDataObject* BP_TryResolveData(USSSettingDataObject* Target, FName InIdentifier, TSubclassOf<USSSettingDataObject> DataClass, bool& bValid);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static void CommonModalNavigationRules(UWidget* Widget);

	/**
	 * Determines if the given ScrollBox can be scrolled, i.e., whether its content size exceeds the viewport size.
	 *
	 * @param ScrollBox A pointer to the ScrollBox to check. Must be a valid instance.
	 * @return Returns true if the ScrollBox is scrollable; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static bool IsScrollBoxScrollable(UScrollBox* ScrollBox);

	UFUNCTION(BlueprintCallable, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static bool CheckPlatformTraitActive(UObject* WorldContextObject, const FGameplayTag VisibilityTag);

	UFUNCTION(BlueprintCallable, Category = "SS|UI", meta=(WorldContext="WorldContextObject"))
	static bool CheckContainsPlatformTrait(UObject* WorldContextObject, const TArray<FGameplayTag>& VisibilityTags);
};
