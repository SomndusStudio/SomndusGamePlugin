/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSScreenFadeTypes.generated.h"

DECLARE_DELEGATE(FSSScreenFadeDelegate);
DECLARE_DYNAMIC_DELEGATE(FSSScreenFadeDynamicDelegate);

USTRUCT(BlueprintType)
struct FSSScreenFadeParams
{
	GENERATED_BODY()

	/** How long the fade should take, in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreenFade")
	float Time;

	/** Color to fade from. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreenFade")
	FLinearColor FromColor;

	/** Color to fade to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreenFade")
	FLinearColor ToColor;

	/** Delegate executed when the fade is finished. */
	FSSScreenFadeDelegate OnFinished;

	/** Event called when the fade is finished. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "On Finished", Category = "ScreenFade")
	FSSScreenFadeDynamicDelegate OnFinishedDynamic;

	/** True to fade audio volume along with the alpha of the color. */
	UPROPERTY(AdvancedDisplay, EditAnywhere, BlueprintReadWrite, Category = "ScreenFade")
	bool bFadeAudio;

	/** True to continue to fade when the game is paused. */
	UPROPERTY(AdvancedDisplay, EditAnywhere, BlueprintReadWrite, Category = "ScreenFade")
	bool bFadeWhenPaused;

	FSSScreenFadeParams()
		: Time(0.0f)
		, FromColor(FLinearColor::Transparent)
		, ToColor(FLinearColor::Transparent)
		, bFadeAudio(true)
		, bFadeWhenPaused(true)
	{
	}

	FSSScreenFadeParams(const float InTime, const FLinearColor& InFromColor, const FLinearColor& InToColor, const FSSScreenFadeDelegate& InOnFinished, const bool bInFadeAudio, const bool bInFadeWhenPaused)
		: Time(InTime)
		, FromColor(InFromColor)
		, ToColor(InToColor)
		, OnFinished(InOnFinished)
		, bFadeAudio(bInFadeAudio)
		, bFadeWhenPaused(bInFadeWhenPaused)
	{
	}

	FSSScreenFadeParams(const float InTime, const FLinearColor& InFromColor, const FLinearColor& InToColor, const FSSScreenFadeDynamicDelegate& InOnFinished, const bool bInFadeAudio, const bool bInFadeWhenPaused)
		: Time(InTime)
		, FromColor(InFromColor)
		, ToColor(InToColor)
		, OnFinishedDynamic(InOnFinished)
		, bFadeAudio(bInFadeAudio)
		, bFadeWhenPaused(bInFadeWhenPaused)
	{
	}
};
