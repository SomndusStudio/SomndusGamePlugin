/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonButtonBase.generated.h"

class USoundBase;
class UAudioComponent;

/**
 * USSCommonButtonBase
 *
 * A UI button class derived from UCommonButtonBase that provides enhanced support 
 * for input action interactions, including audio feedback during hold states.
 * Designed for advanced user interface implementations requiring contextual action responses.
 */
UCLASS()
class SOMNDUSGAME_API USSCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> HoldingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> HoldingAudioComp;

	virtual void HoldReset() override;
	virtual void NativeOnPressed() override;
	virtual void NativeOnReleased() override;
	
	void StopHoldingAudio() const;
};
