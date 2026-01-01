/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSDigitImageWidget.h"
#include "Blueprint/UserWidget.h"
#include "SSNumberDisplayWidget.generated.h"

class UHorizontalBox;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSNumberDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview", meta = (ClampMin = "0"))
	int32 DefaultPreviewNumber = 1234;
	
	/**
	 * Set the number to display, reusing existing digits when possible
	 * @param Number 
	 */
	UFUNCTION(BlueprintCallable, Category = "Number")
	void SetNumber(int32 Number);
	 
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	/**
	 * Bind in UMG designer: a HorizontalBox that holds DigitImageWidgets
	 */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* DigitBox;

	/**
	 * Digit widget CDO to create instances
	 */
	UPROPERTY(EditAnywhere, Category = "Number")
	TSubclassOf<USSDigitImageWidget> DigitWidgetClass;
	 
private:
	/**
	 * Pool of digit widgets for reuse
	 */
	UPROPERTY(Transient)
	TArray<USSDigitImageWidget*> DigitWidgets;
	 
	void UpdateDigits(int32 Number);
};
