// ©2025 Schartier Isaac. All rights reserved.


#include "UI/Button/SSCommonBoundActionButton.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USSCommonBoundActionButton::HoldReset()
{
	Super::HoldReset();
	StopHoldingAudio();
}

void USSCommonBoundActionButton::NativeOnActionProgress(float HeldPercent)
{
	// if not rollback
	if (CurrentHoldProgress < HeldPercent)
	{
		if (!bHoldingSoundIsPlaying)
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
			bHoldingSoundIsPlaying = true;
		}
	}
	else
	{
		if (bHoldingSoundIsPlaying)
		{
			StopHoldingAudio();
		}
	}
	
	Super::NativeOnActionProgress(HeldPercent);
}

void USSCommonBoundActionButton::NativeOnReleased()
{
	Super::NativeOnReleased();
	StopHoldingAudio();
}

void USSCommonBoundActionButton::StopHoldingAudio()
{
	bHoldingSoundIsPlaying = false;
	
	if (HoldingAudioComp)
	{
		HoldingAudioComp->Stop();
	}
}
