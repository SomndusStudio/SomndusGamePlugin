/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "SSInputActionWidget.generated.h"

class STextBlock;
/**
 * This ActionWidget supports automatic keyboard input if a key image is not found.
 * This is useful because there are many localized keyboard layouts, and developers may not want to
 * specify like over 200 images to support all possible keyboard input images in the keyboard input controller settings.
 */
UCLASS()
class SOMNDUSGAME_API USSInputActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:
	/** Begin UWidget */
	virtual TSharedRef<SWidget> RebuildWidget() override;
	/** End UWidget */

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsEmptyKeyboard();
	
protected:

	/**
	 * Try to update automatic keyboard
	 */
	virtual bool UpdateKeyboardAction(const UCommonInputSubsystem* CommonInputSubsystem);
	
	virtual void UpdateActionWidget() override;

	/** For support automatic keyboard input display */
	TSharedPtr<SImage> KeyboardBackground;
	TSharedPtr<STextBlock> KeyboardTextBlock;

	FText KeyDefaultText;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateBrush KeyboardBackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateFontInfo KeyboardFontStyle;

	UPROPERTY(BlueprintAssignable, Category = CommonActionWidget)
	FOnInputMethodChanged OnInputActionUpdated;
};
