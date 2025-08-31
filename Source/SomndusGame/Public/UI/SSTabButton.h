/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonUITypes.h"
#include "SSTabButton.generated.h"

/**
 * USSTabButton is a user interface component derived from UCommonButtonBase that represents a tab button
 * in the UI. It includes functionality to manage associated menu information.
 */
UCLASS()
class SOMNDUSGAME_API USSTabButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSMenuInfo MenuInfo;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMenuInfo(FSSMenuInfo InMenuInfo);
};
