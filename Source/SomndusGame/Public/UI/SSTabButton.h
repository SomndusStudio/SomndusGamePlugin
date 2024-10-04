/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonUITypes.h"
#include "SSTabButton.generated.h"

/**
 * 
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
