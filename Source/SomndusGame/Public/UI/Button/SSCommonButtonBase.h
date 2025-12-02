// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonButtonBase.generated.h"

class USoundBase;
class UAudioComponent;

/**
 * 
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
