/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSCommonWidgetSlotEntry.h"
#include "SSWidgetSlotEntry_Animation.generated.h"

/**
 * @class USSWidgetSlotEntry_Animation
 * @brief Widget slot entry class that provides animation effects such as hover, select, and disappear.
 *
 * This class extends USSCommonWidgetSlotEntry and implements specific behaviors and visual
 * animations to enhance user experience during interactions such as mouse enter, mouse leave,
 * focus events, and confirmation effects. It contains optional binding for animations and
 * background widgets.
 */
UCLASS()
class SOMNDUSGAME_API USSWidgetSlotEntry_Animation : public USSCommonWidgetSlotEntry
{
	GENERATED_BODY()

public:

	void SetHoverEffect(bool bInActive);
		
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayDisappearEffect();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlaySelectConfirmEffect();

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<USSBackgroundSlotWidget> BackgroundWidget;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> HoverAnimation;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> SelectedConfirmAnimation;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> DisappearAnimation;
};
