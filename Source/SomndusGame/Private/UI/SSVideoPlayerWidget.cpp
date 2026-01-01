/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSVideoPlayerWidget.h"

#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "TimerManager.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Helper/SSHelperStatics.h"

void USSVideoPlayerWidget::OnMediaOpened(FString OpenedUrl)
{
	LastOpenedUrl = OpenedUrl;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick(this, &USSVideoPlayerWidget::DelayedOnMediaOpened);
	}
	else
	{
		// Fallback if GetWorld() are null
		DelayedOnMediaOpened();
	}
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
