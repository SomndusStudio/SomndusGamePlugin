/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSTooltipWidgetBase.h"
#include "SSTooltipManager.generated.h"

/**
 * Structure that defines a pooled tooltip entry used by the tooltip manager system.
 *
 * This struct links a specific invoker widget (WidgetToken) with a corresponding tooltip class.
 * It helps track and reuse tooltip instances per widget, optimizing performance and memory use.
 */
USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSTooltipPool
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidget* WidgetToken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USSTooltipWidgetBase> TooltipClass;
};

/**
 * Tooltip Manager responsible for handling dynamic creation, pooling, display, and cleanup of tooltip widgets in the UI.
 *
 * This class manages the lifecycle of tooltips within the UI system, ensuring they appear near invoking widgets,
 * are efficiently reused (via pooling), and avoid duplication. It supports multiple simultaneous tooltips and provides
 * a clean API to show, hide, and query tooltips either in C++ or Blueprints.
 *
 * Typical usage:
 * - Use ShowTooltip() to display a tooltip for a widget.
 * - Use HideCurrentTooltip() to hide the currently active tooltip.
 * - AddTooltip() and ClearTooltip() allow direct control of active instances.
 */
UCLASS()
class SOMNDUSGAME_API USSTooltipManager : public USSContextObject
{
	GENERATED_BODY()

public:

	/** Internal timer used for delayed pooling operations (e.g. delayed tooltip cleanup). */
	UPROPERTY()
	FTimerHandle TimerHandle;

	/** Pool of tooltip entries, each linked to a widget and a tooltip class. */
	UPROPERTY()
	TArray<FSSTooltipPool> TooltipPools;

	/** List of all distinct tooltip instances managed by this class. */
	UPROPERTY()
	TArray<USSTooltipWidgetBase*> DistinctTooltips;

	/** The widget currently acting as the tooltip invoker. */
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TooltipInvoker;

	/** Currently displayed tooltip widget. */
	UPROPERTY(BlueprintReadOnly)
	USSTooltipWidgetBase* CurrentTooltip;

	/** All tooltips that are currently active and visible. */
	UPROPERTY()
	TArray<USSTooltipWidgetBase*> ActiveTooltips;

protected:
	/**
	 * To know if we should fix/.refresh tooltip position for tick performance
	 */
	FVector2D LastTooltipInvokerPosition;

public:
	bool Tick(float DeltaSeconds);
	
	/**
	 * Adds a tooltip to the list of active tooltips and makes it visible.
	 *
	 * @param TooltipWidget The tooltip widget to activate and manage.
	 */
	UFUNCTION(BlueprintCallable)
	void AddTooltip(USSTooltipWidgetBase* TooltipWidget);

	/**
	 * Hides and clears the currently displayed tooltip.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearTooltip();

	/**
	 * Checks the pool for an existing tooltip of the given class.
	 *
	 * @param TooltipClass Class to search for.
	 * @return The tooltip instance if found, or nullptr.
	 */
	UFUNCTION(BlueprintCallable)
	USSTooltipWidgetBase* CheckTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass);
	
	/**
	 * Calculates and returns the position of a widget in the viewport.
	 *
	 * @param Widget The widget whose position is to be found.
	 * @return The screen position of the widget.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static FVector2D GetWidgetPositionInViewport(UUserWidget* Widget);

	static bool TryFixTooltip(UObject* WorldContextObject, UUserWidget* InvokerWidget, UUserWidget* TooltipWidget);

	static FVector2D GetWidgetInvokerPosition(UUserWidget* Widget, UUserWidget* Invoker);
	/**
	 * Positions a widget relative to the invoker widget in the viewport.
	 *
	 * @param Widget The widget to position (e.g. a tooltip).
	 * @param Invoker The widget that triggered the tooltip.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	FVector2D PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker);
	
	/** Internal function called after a delay to manage the tooltip pool (e.g. cleanup). */
	void OnDelayCheckPool();
	
	void RemoveTooltipWidget(USSTooltipWidgetBase* TooltipWidget);
	void RemoveTooltipPool(const TSubclassOf<USSTooltipWidgetBase>& Class, UUserWidget* Invoker, bool bForceCurrentTick=false);
	void ShowTooltipWidget(USSTooltipWidgetBase* TooltipWidget, UUserWidget* Invoker, UObject* DataObject, bool Active=true);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	USSTooltipWidgetBase* ShowTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker, UObject* DataObject, bool Active=true);

	/**
	 * Hides the current active tooltip.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	void HideCurrentTooltip();

	/**
	 * Retrieves a tooltip instance from the pool by class.
	 *
	 * @param TooltipClass The tooltip class to retrieve.
	 * @return A tooltip instance of the specified class or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "Tooltip",
		meta = (DeterminesOutputType = "TooltipClass"))
	USSTooltipWidgetBase* GetTooltipByClass(TSubclassOf<USSTooltipWidgetBase> TooltipClass);

	/**
	 * Template version of GetTooltipByClass for strongly-typed casting.
	 * 
	 * Example usage:
	 *   auto* MyTooltip = TooltipManager->GetTooltipByClass<UMyCustomTooltip>();
	 */
	template <class T>
	T* GetTooltipByClass()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UUserWidget>::Value,
		              "'T' template parameter to GetTooltipByClass must be derived from UUserWidget");

		return static_cast<T*>(GetTooltipByClass(T::StaticClass()));
	}
};
