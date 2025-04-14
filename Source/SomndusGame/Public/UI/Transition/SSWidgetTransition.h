// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSWidgetTransition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSWidgetTransitionEventSignature, class USSWidgetTransition*, WidgetTransition);

DECLARE_DELEGATE(FSSTransitionFadeDelegate);
DECLARE_DYNAMIC_DELEGATE(FSSTransitionFadeDynamicDelegate);

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSTransitionParams : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AFT")
	float PlayRate = 1.0f;
};


/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSWidgetTransition : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void StartFadeIn(const FSSTransitionParams& Params);

	UFUNCTION(BlueprintNativeEvent)
	void StartFadeOut(const FSSTransitionParams& Params);

	UFUNCTION(BlueprintNativeEvent)
	void StartFade(const FSSTransitionParams& Params);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS")
	void NotifyFinished();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="AFT|EventDispatchers")
	FSSWidgetTransitionEventSignature OnFinished;
};
