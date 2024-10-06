/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSTooltipWidgetBase.h"
#include "SSTooltipManager.generated.h"

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
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSTooltipManager : public USSContextObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	TArray<FSSTooltipPool> TooltipPools;

	UPROPERTY()
	TArray<USSTooltipWidgetBase*> DistinctTooltips;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TooltipInvoker;

	UPROPERTY(BlueprintReadOnly)
	USSTooltipWidgetBase* CurrentTooltip;

	UPROPERTY()
	TArray<USSTooltipWidgetBase*> ActiveTooltips;

	UFUNCTION(BlueprintCallable)
	void AddTooltip(USSTooltipWidgetBase* TooltipWidget);

	UFUNCTION(BlueprintCallable)
	void ClearTooltip();

	UFUNCTION(BlueprintCallable)
	USSTooltipWidgetBase* CheckTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static FVector2D GetWidgetPositionInViewport(UUserWidget* Widget);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PositionWidgetToInvoker(UUserWidget* Widget, UUserWidget* Invoker);

	void OnDelayCheckPool();
	
	void RemoveTooltipWidget(USSTooltipWidgetBase* TooltipWidget);
	void RemoveTooltipPool(const TSubclassOf<USSTooltipWidgetBase>& Class, UUserWidget* Invoker);
	void ShowTooltipWidget(USSTooltipWidgetBase* TooltipWidget, UUserWidget* Invoker, UObject* DataObject, bool Active=true);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	USSTooltipWidgetBase* ShowTooltip(TSubclassOf<USSTooltipWidgetBase> TooltipClass, UUserWidget* Invoker, UObject* DataObject, bool Active=true);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI", meta=(WorldContext="WorldContextObject", DeterminesOutputType = "TooltipClass"))
	void HideCurrentTooltip();
	
	UFUNCTION(BlueprintPure, Category = "Tooltip",
		meta = (DeterminesOutputType = "TooltipClass"))
	USSTooltipWidgetBase* GetTooltipByClass(TSubclassOf<USSTooltipWidgetBase> TooltipClass);

	/** Templatized version of GetTooltipByClass that handles casting for you */
	template <class T>
	T* GetTooltipByClass()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UUserWidget>::Value,
		              "'T' template parameter to GetTooltipByClass must be derived from UUserWidget");

		return (T*)GetTooltipByClass(T::StaticClass());
	}
};
