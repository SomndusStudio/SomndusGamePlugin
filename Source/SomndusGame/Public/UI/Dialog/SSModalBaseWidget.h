﻿// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SSGameActivatableWidget.h"
#include "SSModalBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSModalBaseWidget : public USSGameActivatableWidget
{
	GENERATED_BODY()

	USSModalBaseWidget();

protected:
	
	virtual void NativePreConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> ShowAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidget> OverlayModalRoot;
};
