/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSWidgetTransition.h"
#include "SSWidgetTransitionMgr.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSWidgetTransitionMgrEventSignature, class USSWidgetTransitionMgr*, WidgetTransitionMgr);

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSWidgetTransitionMgr : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USSWidgetTransition> CurrentWidgetTransition;

	UFUNCTION(BlueprintPure)
	USSWidgetTransition* GetCurrentWidgetTransition() { return CurrentWidgetTransition; }
	
	void CreateWidgetTransition(TSubclassOf<USSWidgetTransition> Class);

	bool CheckWidgetTransition(TSubclassOf<USSWidgetTransition> Class);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS")
	bool PlayTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams = FSSTransitionParams());
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS")
	bool FadeInTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams = FSSTransitionParams());

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS")
	bool FadeOutTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams = FSSTransitionParams());
	
	UFUNCTION()
	void NativeOnTransitionFinished(USSWidgetTransition* WidgetTransition);
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="AFT|EventDispatchers")
	FSSWidgetTransitionMgrEventSignature OnTransitionFinished;
};
