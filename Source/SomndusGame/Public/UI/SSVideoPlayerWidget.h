/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSVideoPlayerWidget.generated.h"

class UOverlay;
class UImage;
class UMediaPlaylist;
class UFileMediaSource;
class UMediaPlayer;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSVideoPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMediaPlayer> MediaPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFileMediaSource> FileMediaSource;

	TWeakPtr<FActiveTimerHandle> ActiveTimerHandle;
	
	/**
	 * Actually used as fix for console (one video file loop)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMediaPlaylist> MediaPlaylist;

	UPROPERTY(BlueprintReadWrite)
	FString LastVideoFileName;

	UPROPERTY(BlueprintReadOnly)
	FString LastOpenedUrl;

	UPROPERTY(meta=(BindWidget))
	UImage* ImageVideo;
	
	UPROPERTY(meta=(BindWidget))
	UOverlay* OverlayNoData;

	EActiveTimerReturnType OnTimerDelayedOnMediaOpened(double InCurrentTime, float InDeltaTime);
	 
	void StopSlateTimer();
	
public:
	
	UFUNCTION()
	void OnMediaOpened(FString OpenedUrl);

	void DelayedOnMediaOpened();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnMediaOpened();
	
	UFUNCTION(BlueprintCallable)
	void Clear();
	
	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void RefreshVideo();

	UFUNCTION(BlueprintCallable)
	void PlayVideo(FString Filename);

	UFUNCTION(BlueprintCallable)
	void StopVideo();

	UFUNCTION(BlueprintCallable)
	void RefreshNoData(bool bValid);
	
	UFUNCTION(BlueprintCallable)
	void ActiveMediaPlayer();

	UFUNCTION(BlueprintCallable)
	void DeactivateMediaPlayer();
};
