// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "SSScreenFadeWidget.h"
#include "AudioDevice.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Misc/EngineVersionComparison.h"

void SSSScreenFadeWidget::Construct(const FArguments& InArgs)
{
	FadeParams = InArgs._FadeParams;
	OnFadeFinished = InArgs._OnFadeFinished;
}

void SSSScreenFadeWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!FadeParams.bFadeWhenPaused && IsGamePaused())
	{
		return;
	}

	TimeRemaining = FMath::Max(TimeRemaining - InDeltaTime, 0.0f);

	if (TimeRemaining == 0.0f)
	{
		FinishFade();
		return;
	}

	const FLinearColor NextColor = FadeParams.ToColor - (FadeParams.ToColor - FadeParams.FromColor) * TimeRemaining / FadeParams.Time;
	ApplyFade(NextColor);
}

void SSSScreenFadeWidget::StartFade()
{
	SetImage(FCoreStyle::Get().GetBrush("WhiteBrush"));

	if (FadeParams.Time <= 0.0f)
	{
		FinishFade();
		return;
	}

	TimeRemaining = FadeParams.Time;
	ApplyFade(FadeParams.FromColor);

	SetCanTick(true);
}

void SSSScreenFadeWidget::FinishFade()
{
	ApplyFade(FadeParams.ToColor);

	OnFadeFinished.ExecuteIfBound();
	FadeParams.OnFinished.ExecuteIfBound();
	FadeParams.OnFinishedDynamic.ExecuteIfBound();

	SetCanTick(false);
}

void SSSScreenFadeWidget::ApplyFade(const FLinearColor& NextColor)
{
	SetColorAndOpacity(NextColor);

	if (FadeParams.bFadeAudio)
	{
		SetPrimaryVolume(1.0f - NextColor.A);
	}
}

UWorld* SSSScreenFadeWidget::GetWorld() const
{
	if (GEngine)
	{
		if (const UGameViewportClient* GameViewport = GEngine->GameViewport)
		{
			return GameViewport->GetWorld();
		}
	}

	return nullptr;
}

bool SSSScreenFadeWidget::IsGamePaused() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->IsPaused();
	}

	return false;
}

void SSSScreenFadeWidget::SetPrimaryVolume(const float Volume)
{
	if (UWorld* World = GetWorld())
	{
		if (FAudioDevice* AudioDevice = World->GetAudioDeviceRaw())
		{
#if UE_VERSION_OLDER_THAN(5, 1, 0)
			AudioDevice->SetTransientMasterVolume(Volume);
#else
			AudioDevice->SetTransientPrimaryVolume(Volume);
#endif
		}
	}
}
