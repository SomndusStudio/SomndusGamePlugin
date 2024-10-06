/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSCursorWidgetBase.h"
#include "SSCursorManager.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class SOMNDUSGAME_API USSCursorManager : public USSContextObject
{
	GENERATED_BODY()

public:

	void Init(const class USSGameUIManagerSubsystem* GameUIManagerSubsystem);

	UPROPERTY(EditAnywhere)
	TObjectPtr<USSCursorWidgetBase> MainCursor;
	
	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<USSCursorWidgetBase> CursorClass;

	void ShowCursor(UUserWidget* Invoker);
	void HideCursor(UUserWidget* Invoker);
};
