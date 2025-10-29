/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "SSWidgetTransition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSWidgetTransitionEventSignature, class USSWidgetTransition*, WidgetTransition);

DECLARE_DELEGATE(FSSTransitionFadeDelegate);
DECLARE_DYNAMIC_DELEGATE(FSSTransitionFadeDynamicDelegate);

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSTransitionParams : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Somndus Studio")
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
