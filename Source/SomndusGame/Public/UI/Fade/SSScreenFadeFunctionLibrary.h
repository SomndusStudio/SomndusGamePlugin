/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSScreenFadeTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSScreenFadeFunctionLibrary.generated.h"

UCLASS()
class SOMNDUSGAME_API USSScreenFadeFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Does a screen fade to/from a color.
	 * @param FadeParams - Parameters to set fade.
	 * @param OwningPlayer - The player to add the fade widget to.
	 * @param ZOrder - The Z-order index for this fade widget. Larger values will cause the widget to appear on top of widgets with lower values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "ScreenFade", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 2))
	static void Fade(
		const UObject* WorldContextObject,
		const FSSScreenFadeParams& FadeParams,
		const APlayerController* OwningPlayer = nullptr,
		const int32 ZOrder = 100);

	/**
	 * Does a screen fade from a color to transparent.
	 * @param Time - How long the fade should take, in seconds.
	 * @param FromColor - Color to fade from.
	 * @param OnFinished - Delegate executed when the fade is finished.
	 * @param bFadeAudio - True to fade audio volume along with the alpha of the color.
	 * @param bFadeWhenPaused - True to continue to fade when the game is paused.
	 * @param OwningPlayer - The player to add the fade widget to.
	 * @param ZOrder - The Z-order index for this fade widget. Larger values will cause the widget to appear on top of widgets with lower values.
	 */
	static void FadeIn(
		const UObject* WorldContextObject,
		const float Time = 1.0f,
		const FLinearColor& FromColor = FLinearColor::Black,
		const FSSScreenFadeDelegate& OnFinished = FSSScreenFadeDelegate(),
		const bool bFadeAudio = true,
		const bool bFadeWhenPaused = true,
		const APlayerController* OwningPlayer = nullptr,
		const int32 ZOrder = 100);

	/**
	 * Does a screen fade from transparent to a color.
	 * @param Time - How long the fade should take, in seconds.
	 * @param ToColor - Color to fade to.
	 * @param OnFinished - Delegate executed when the fade is finished.
	 * @param bFadeAudio - True to fade audio volume along with the alpha of the color.
	 * @param bFadeWhenPaused - True to continue to fade when the game is paused.
	 * @param OwningPlayer - The player to add the fade widget to.
	 * @param ZOrder - The Z-order index for this fade widget. Larger values will cause the widget to appear on top of widgets with lower values.
	 */
	static void FadeOut(
		const UObject* WorldContextObject,
		const float Time = 1.0f,
		const FLinearColor& ToColor = FLinearColor::Black,
		const FSSScreenFadeDelegate& OnFinished = FSSScreenFadeDelegate(),
		const bool bFadeAudio = true,
		const bool bFadeWhenPaused = true,
		const APlayerController* OwningPlayer = nullptr,
		const int32 ZOrder = 100);

private:
	/**
	 * Does a screen fade from a color to transparent.
	 * @param Time - How long the fade should take, in seconds.
	 * @param FromColor - Color to fade from.
	 * @param OnFinished - Event called when the fade is finished.
	 * @param bFadeAudio - True to fade audio volume along with the alpha of the color.
	 * @param bFadeWhenPaused - True to continue to fade when the game is paused.
	 * @param OwningPlayer - The player to add the fade widget to.
	 * @param ZOrder - The Z-order index for this fade widget. Larger values will cause the widget to appear on top of widgets with lower values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "ScreenFade", meta = (DisplayName = "Fade In", WorldContext = "WorldContextObject", Time = "1.0", FromColor = "(R=0.0,G=0.0,B=0.0,A=1.0)", AutoCreateRefTerm = "OnFinished", AdvancedDisplay = 4))
	static void K2_FadeIn(
		const UObject* WorldContextObject,
		const float Time,
		const FLinearColor FromColor,
		const FSSScreenFadeDynamicDelegate& OnFinished,
		const bool bFadeAudio = true,
		const bool bFadeWhenPaused = true,
		const APlayerController* OwningPlayer = nullptr,
		const int32 ZOrder = 100);

	/**
	 * Does a screen fade from transparent to a color.
	 * @param Time - How long the fade should take, in seconds.
	 * @param ToColor - Color to fade to.
	 * @param OnFinished - Event called when the fade is finished.
	 * @param bFadeAudio - True to fade audio volume along with the alpha of the color.
	 * @param bFadeWhenPaused - True to continue to fade when the game is paused.
	 * @param OwningPlayer - The player to add the fade widget to.
	 * @param ZOrder - The Z-order index for this fade widget. Larger values will cause the widget to appear on top of widgets with lower values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "ScreenFade", meta = (DisplayName = "Fade Out", WorldContext = "WorldContextObject", Time = "1.0", ToColor = "(R=0.0,G=0.0,B=0.0,A=1.0)", AutoCreateRefTerm = "OnFinished", AdvancedDisplay = 4))
	static void K2_FadeOut(
		const UObject* WorldContextObject,
		const float Time,
		const FLinearColor ToColor,
		const FSSScreenFadeDynamicDelegate& OnFinished,
		const bool bFadeAudio = true,
		const bool bFadeWhenPaused = true,
		const APlayerController* OwningPlayer = nullptr,
		const int32 ZOrder = 100);
};
