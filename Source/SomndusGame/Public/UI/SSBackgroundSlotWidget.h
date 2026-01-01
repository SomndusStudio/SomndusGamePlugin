/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSBackgroundSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSBackgroundSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsHovering = false;
	
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool InSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSelectedChange(bool bInSelected = true);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHoverActive(bool InActive = true);

private:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> HoverAnimation;
};
