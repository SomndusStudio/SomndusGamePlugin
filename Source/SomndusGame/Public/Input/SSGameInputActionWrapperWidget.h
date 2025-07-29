// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSGameInputActionWrapperWidget.generated.h"

class UCommonTextBlock;
class USSGameInputActionWidget;
class UInputAction;
/**
 * A UUserWidget wrapper that embeds a USSGameInputActionWidget for use in dynamic entry boxes.
 */
UCLASS()
class SOMNDUSGAME_API USSGameInputActionWrapperWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 * Sets the input action on the internal widget.
	 * @param InInputAction The input action to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputAction(UInputAction* InInputAction);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetDisplayName(FText DisplayText);
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	USSGameInputActionWidget* GetInputActionWidget() const;
	
private:
	/** Instance of the input action widget */
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USSGameInputActionWidget> InputActionWidget;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> DisplayNameTextBlock;
	
	/** Cached input action to apply on construct */
	UPROPERTY(Transient)
	TObjectPtr<UInputAction> PendingInputAction;
};
