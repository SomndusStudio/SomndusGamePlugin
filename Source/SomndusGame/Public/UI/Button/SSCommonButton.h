// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "NativeGameplayTags.h"
#include "SSCommonButton.generated.h"

namespace SSGameplayTags
{
	SOMNDUSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Input_Token_UI_Button);
}

/**
 * USSCommonButton is a custom button class that extends UCommonButtonBase.
 * It provides additional functionality for handling click feedback effects
 * and input suspension during click effects.
 */
UCLASS()
class SOMNDUSGAME_API USSCommonButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeOnClicked() override;
	
	UFUNCTION()
	virtual void OnClickFXFinished();

protected:
	
	UPROPERTY(EditAnywhere, Category="Button|Click")
	bool bUseClickFXDelay = false;

	UPROPERTY(EditAnywhere, Category="Button|Click")
	bool bUseClickAnimation = false;
	
	UPROPERTY(EditAnywhere, Category="Button|Click", meta=(editcondition="bUseClickFXDelay"))
	float ClickFXDelay = 0.1f;
	
	UPROPERTY(EditAnywhere, Category="Button|Click", meta=(editcondition="bUseClickFXDelay"))
	bool bSuspendInputClickFX = false;

	UPROPERTY()
	FTimerHandle ClickFXHandle;

	UPROPERTY()
	FWidgetAnimationDynamicEvent ClickAnimEndDelegate;
	
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional), BlueprintReadOnly)
	UWidgetAnimation* ClickAnimation;
};
