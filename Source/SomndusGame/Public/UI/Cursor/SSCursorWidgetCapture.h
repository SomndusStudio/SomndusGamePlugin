/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/SSCommonUITypes.h"
#include "SSCursorWidgetCapture.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCursorWidgetCapture : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSCursorRequestInfo CursorInfo;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	FSSCursorRequestInfo GetCursorRequestInfo();
};
