/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSVideoPlayerWidget.h"

#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Helper/SSHelperStatics.h"

EActiveTimerReturnType USSVideoPlayerWidget::OnTimerDelayedOnMediaOpened(double InCurrentTime, float InDeltaTime)
{
	StopSlateTimer();

	DelayedOnMediaOpened();

	return EActiveTimerReturnType::Stop;
}

void USSVideoPlayerWidget::StopSlateTimer()
{
	if (ActiveTimerHandle.IsValid())
	{
		TakeWidget()->UnRegisterActiveTimer(ActiveTimerHandle.Pin().ToSharedRef());
		ActiveTimerHandle.Reset();
	}
}

void USSVideoPlayerWidget::OnMediaOpened(FString OpenedUrl)
{
	LastOpenedUrl = OpenedUrl;

	ActiveTimerHandle = TakeWidget()->RegisterActiveTimer(
		0.f, FWidgetActiveTimerDelegate::CreateUObject(this, &USSVideoPlayerWidget::OnTimerDelayedOnMediaOpened)
	);
}

void USSVideoPlayerWidget::DelayedOnMediaOpened()
{
	RefreshVideo();

	BP_OnMediaOpened();
}

void USSVideoPlayerWidget::Clear()
{
	LastVideoFileName = "";
}

void USSVideoPlayerWidget::Reload()
{
	if (!MediaPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("USSVideoPlayerWidget::Reload() - MediaPlayer it's not valid "));
		return;
	}

	MediaPlayer->Play();
}

void USSVideoPlayerWidget::RefreshVideo()
{
	const FIntPoint VideoDimension = MediaPlayer->GetVideoTrackDimensions(0, 0);

	ImageVideo->SetDesiredSizeOverride(VideoDimension);

	MediaPlayer->SetLooping(true);

	MediaPlayer->Play();
}

void USSVideoPlayerWidget::PlayVideo(FString Filename)
{
	LastVideoFileName = Filename;

	FString Path = USSHelperStatics::ConcatStrings("./Movies/", LastVideoFileName);
	FileMediaSource->SetFilePath(Path);

	bool bOpenPlaylistSuccess = MediaPlayer->OpenPlaylist(MediaPlaylist);

	RefreshNoData(bOpenPlaylistSuccess);
}

void USSVideoPlayerWidget::StopVideo()
{
	MediaPlayer->Close();

	ImageVideo->SetColorAndOpacity(FLinearColor::Black);

	RefreshNoData(false);
}

void USSVideoPlayerWidget::RefreshNoData(bool bValid)
{
	OverlayNoData->SetVisibility(bValid ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void USSVideoPlayerWidget::ActiveMediaPlayer()
{
	MediaPlayer->OnMediaOpened.AddUniqueDynamic(this, &ThisClass::OnMediaOpened);

	if (!LastVideoFileName.IsEmpty())
	{
		PlayVideo(LastVideoFileName);
	}
}

void USSVideoPlayerWidget::DeactivateMediaPlayer()
{
	MediaPlayer->OnMediaOpened.RemoveDynamic(this, &ThisClass::OnMediaOpened);
}
