/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "SSDigitImageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSDigitImageWidget : public UImage
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Digit")
	void SetDigit(int32 Digit);
	 
protected:
	virtual void SynchronizeProperties() override;
	 
private:

	int32 CurrentDigit = 0;
};
