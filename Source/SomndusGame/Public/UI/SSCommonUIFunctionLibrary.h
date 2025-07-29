/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Setting/SSSettingDataObject.h"
#include "Tooltip/SSTooltipWidgetBase.h"
#include "SSCommonUIFunctionLibrary.generated.h"

class USSGameNotificationManager;
/**
 * 
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
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	static USSTooltipWidgetBase* ShowTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker, UObject* DataObject, bool Active=true);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void HideTooltip(UObject* WorldContextObject, TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void HideCurrentTooltip(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void ShowCursor(UObject* WorldContextObject, UUserWidget* Invoker);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void HideCursor(UObject* WorldContextObject, UUserWidget* Invoker);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetAbsolutePosition(UUserWidget* UserWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void AbsoluteToViewport(UUserWidget* UserWidget, FVector2D AbsoluteDesktopCoordinate, FVector2D& PixelPosition, FVector2D& ViewportPosition);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static UWidgetAnimation* TryGetWidgetAnimation(UUserWidget* UserWidget, FName InAnimationName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetTopLeftPosition(UUserWidget* UserWidget);
	
	UFUNCTION(BlueprintCallable, Category="SomndusGame", DisplayName="TryResolveData", meta=(DeterminesOutputType = "DataClass", ExpandBoolAsExecs="bValid"))
	static USSSettingDataObject* BP_TryResolveData(USSSettingDataObject* Target, FName InIdentifier, TSubclassOf<USSSettingDataObject> DataClass, bool& bValid);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject"))
	static void CommonModalNavigationRules(UWidget* Widget);
	
};
