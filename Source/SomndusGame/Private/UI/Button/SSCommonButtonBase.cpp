/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Button/SSCommonButtonBase.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USSCommonButtonBase::HoldReset()
{
	Super::HoldReset();
	StopHoldingAudio();
}

void USSCommonButtonBase::NativeOnPressed()
{
	Super::NativeOnPressed();
	if (bRequiresHold && HoldTime > 0.f)
	{
		if (HoldingSound)
		{
			// if should create audio comp first time
			if (!HoldingAudioComp)
			{
				HoldingAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), HoldingSound, 1.f, 1.f);
			}
			// if not yet played for any reason play it
			if (!HoldingAudioComp->IsPlaying())
			{
				HoldingAudioComp->Play(0.f);
			}
		}
	}
}

void USSCommonButtonBase::NativeOnReleased()
{
	Super::NativeOnReleased();
	StopHoldingAudio();
}

void USSCommonButtonBase::StopHoldingAudio() const
{
	if (HoldingAudioComp)
	{
		HoldingAudioComp->Stop();
	}
}
