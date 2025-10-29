// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSInputAxisLoggerWidget.generated.h"

class UImage;
class UTextBlock;
class UOverlay;
class USSInputAxisMgr;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputAxisLoggerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInputAxisMgr(USSInputAxisMgr* InMgr);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float MaxRange = 256.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FVector2D DotCenterOffset = FVector2D(9, 9);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* AnalogOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* AnalogStickDot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveInputText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InputAngleText;

private:
	UPROPERTY()
	TObjectPtr<USSInputAxisMgr> InputAxisMgr;
};
