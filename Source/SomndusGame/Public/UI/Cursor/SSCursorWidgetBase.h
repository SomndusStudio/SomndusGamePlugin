/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/SSCommonUITypes.h"
#include "SSCursorWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCursorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* Invoker;
	
	FDelegateHandle DestructDelegateHandle;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void HandleWidgetDestruct(UUserWidget* UserWidget);
	
	void NativeUpdateCursorInfo(UUserWidget* InInvoker);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCursorInfo(FSSCursorRequestInfo CursorRequestInfo);
};
