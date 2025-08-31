/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSButtonText.h"
#include "UI/Cursor/SSCursorHandler.h"
#include "UI/Cursor/SSCursorWidgetCapture.h"
#include "SSButtonText_WithCursor.generated.h"

/**
 * USSButtonText_WithCursor is a specialized button class that enhances USSButtonText with cursor handling
 * capabilities. It also implements the ISSCursorHandler interface to provide specific cursor behavior.
 */
UCLASS()
class SOMNDUSGAME_API USSButtonText_WithCursor : public USSButtonText, public ISSCursorHandler
{
	GENERATED_BODY()

public:
	virtual FSSCursorRequestInfo GetCursorRequestInfo_Implementation() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USSCursorWidgetCapture> HandCursorCapture;
};
