/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "UI/Fade/SSScreenFadeFunctionLibrary.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "UI/SSGameUIManagerSubsystem.h"

void USSScreenFadeFunctionLibrary::Fade(const UObject* WorldContextObject, const FSSScreenFadeParams& FadeParams, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			USSGameUIManagerSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<USSGameUIManagerSubsystem>();
			Subsystem->AddFadeWidget(FadeParams, OwningPlayer, ZOrder);
		}
	}
}

void USSScreenFadeFunctionLibrary::FadeIn(const UObject* WorldContextObject, const float Time, const FLinearColor& FromColor, const FSSScreenFadeDelegate& OnFinished, const bool bFadeAudio, const bool bFadeWhenPaused, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	const FSSScreenFadeParams FadeParams(Time, FromColor, FLinearColor::Transparent, OnFinished, bFadeAudio, bFadeWhenPaused);
	Fade(WorldContextObject, FadeParams, OwningPlayer, ZOrder);
}

void USSScreenFadeFunctionLibrary::FadeOut(const UObject* WorldContextObject, const float Time, const FLinearColor& ToColor, const FSSScreenFadeDelegate& OnFinished, const bool bFadeAudio, const bool bFadeWhenPaused, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	const FSSScreenFadeParams FadeParams(Time, FLinearColor::Transparent, ToColor, OnFinished, bFadeAudio, bFadeWhenPaused);
	Fade(WorldContextObject, FadeParams, OwningPlayer, ZOrder);
}

void USSScreenFadeFunctionLibrary::K2_FadeIn(const UObject* WorldContextObject, const float Time, const FLinearColor FromColor, const FSSScreenFadeDynamicDelegate& OnFinished, const bool bFadeAudio, const bool bFadeWhenPaused, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	const FSSScreenFadeParams FadeParams(Time, FromColor, FLinearColor::Transparent, OnFinished, bFadeAudio, bFadeWhenPaused);
	Fade(WorldContextObject, FadeParams, OwningPlayer, ZOrder);
}

void USSScreenFadeFunctionLibrary::K2_FadeOut(const UObject* WorldContextObject, const float Time, const FLinearColor ToColor, const FSSScreenFadeDynamicDelegate& OnFinished, const bool bFadeAudio, const bool bFadeWhenPaused, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	const FSSScreenFadeParams FadeParams(Time, FLinearColor::Transparent, ToColor, OnFinished, bFadeAudio, bFadeWhenPaused);
	Fade(WorldContextObject, FadeParams, OwningPlayer, ZOrder);
}
