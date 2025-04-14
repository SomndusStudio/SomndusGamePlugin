// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Components/Widget.h"
#include "SSGameInputActionWidget.generated.h"

class UCommonInputSubsystem;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOMNDUSGAME_API USSGameInputActionWidget : public UWidget
{
	GENERATED_BODY()

protected:
	UCommonInputSubsystem* GetInputSubsystem() const;
	
	/** Begin UWidget */
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/** End UWidet */

	UFUNCTION(BlueprintCallable)
	void Refresh();
	
	virtual void UpdateInputActionWidget();
	/**
	 * Try to update automatic keyboard
	 */
	virtual bool UpdateKeyboardAction(const UCommonInputSubsystem* CommonInputSubsystem);
	
	void ListenToInputMethodChanged(bool bListen = true);
	void HandleInputMethodChanged(ECommonInputType InInputType);
	
	virtual bool ShouldUpdateActionWidgetIcon() const;
	
	UFUNCTION(BlueprintCallable, Category = CommonActionWidget)
	virtual FSlateBrush GetIcon() const;
	
	/** For support automatic keyboard input display */
	TSharedPtr<SImage> KeyboardBackground;
	TSharedPtr<STextBlock> KeyboardTextBlock;

	FText KeyDefaultText;

	TSharedPtr<SBox> MyKeyBox;

	TSharedPtr<SImage> MyIcon;

	FSlateBrush Icon;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputAction(class UInputAction* InInputAction);
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Input")
	FKey DesignTimeKey;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<class UInputAction> InputAction;

	/**
	 * Force input type (keyboard/gamepad, etc)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool bForceInputType = false;
	
	/**
	 * Used to force display only gamepad key
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool bGamepad = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateBrush KeyboardBackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateFontInfo KeyboardFontStyle;
};
