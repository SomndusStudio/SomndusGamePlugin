// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSWidgetTransition.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SSWidgetTransitionAsyncAction.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSWidgetTransitionAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	
	virtual void Activate() override;

private:
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "ScreenFade", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 2))
	static USSWidgetTransitionAsyncAction* AsyncFade(
		UObject* WorldContextObject,
		TSubclassOf<USSWidgetTransition> InWidgetTransitionClass,
		const FSSTransitionParams& FadeParams,
		const int32 ZOrder = 100);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|WidgetTransition", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 3))
	static USSWidgetTransitionAsyncAction* AsyncFadeIn(
		UObject* WorldContextObject,
		TSubclassOf<USSWidgetTransition> InWidgetTransitionClass,
		const FSSTransitionParams& FadeParams,
		const int32 ZOrder = 100);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|WidgetTransition", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 3))
	static USSWidgetTransitionAsyncAction* AsyncFadeOut(
		UObject* WorldContextObject,
		TSubclassOf<USSWidgetTransition> InWidgetTransitionClass,
		const FSSTransitionParams& FadeParams,
		const int32 ZOrder = 100);

	UFUNCTION()
	void NativeOnTransitionFinished(USSWidgetTransitionMgr* WidgetTransitionMgr);

	/**
	 * 0 : Fade in & out
	 * 1 : Fade in
	 * 2 : Fade out
	 */
	int32 FadeType = 0;
	
	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContextObject;
	
	UPROPERTY()
	TSubclassOf<USSWidgetTransition> WidgetTransitionClass;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningPlayer;
	
	FSSTransitionParams FadeParams;
	int32 ZOrder;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishedDelegate);

	/** Called when the fade is finished. */
	UPROPERTY(BlueprintAssignable)
	FFinishedDelegate Finished;
};
