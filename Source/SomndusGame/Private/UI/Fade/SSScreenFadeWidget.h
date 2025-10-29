// Copyright 2023-2024 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Fade/SSScreenFadeTypes.h"
#include "Widgets/Images/SImage.h"

class SSSScreenFadeWidget : public SImage
{
public:
	SLATE_BEGIN_ARGS(SSSScreenFadeWidget)
	{
	}
	SLATE_ARGUMENT(FSSScreenFadeParams, FadeParams)
	SLATE_EVENT(FSSScreenFadeDelegate, OnFadeFinished)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void StartFade();

private:
	void FinishFade();
	void ApplyFade(const FLinearColor& NextColor);

	UWorld* GetWorld() const;
	bool IsGamePaused() const;
	void SetPrimaryVolume(const float Volume);

	FSSScreenFadeParams FadeParams;
	FSSScreenFadeDelegate OnFadeFinished;

	float TimeRemaining;
};
