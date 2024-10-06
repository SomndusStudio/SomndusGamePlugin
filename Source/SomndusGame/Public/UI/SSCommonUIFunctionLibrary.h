/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Tooltip/SSTooltipWidgetBase.h"
#include "SSCommonUIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SS|UI")
	static void SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|UI", meta = (WorldContext = "WorldContextObject"))
	static ULocalPlayer* GetLocalPlayerFromContext(UObject* WorldContextObject);
	
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
	static FVector2D GetTopLeftPosition(UUserWidget* UserWidget);
	
};
