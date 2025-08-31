/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSCursorHandler.h"
#include "SSCursorWidgetCapture.h"
#include "UI/SSCommonWidgetSlotEntry.h"
#include "SSWidgetSlotEntry_WithCursor.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSWidgetSlotEntry_WithCursor : public USSCommonWidgetSlotEntry, public ISSCursorHandler
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
