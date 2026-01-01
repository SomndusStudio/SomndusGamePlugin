/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionButton.h"
#include "SSCommonBoundActionButton.generated.h"

class UAudioComponent;

/**
 * USSCommonBoundActionButton
 *
 * A UI button class derived from UCommonBoundActionButton that provides enhanced support 
 * for input action interactions, including audio feedback during hold states.
 * Designed for advanced user interface implementations requiring contextual action responses.
 */
UCLASS()
class SOMNDUSGAME_API USSCommonBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> HoldingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> HoldingAudioComp;

	bool bHoldingSoundIsPlaying = false;
	
	virtual void HoldReset() override;

	virtual void NativeOnActionProgress(float HeldPercent) override;
	
	virtual void NativeOnReleased() override;
	
	void StopHoldingAudio();
};
