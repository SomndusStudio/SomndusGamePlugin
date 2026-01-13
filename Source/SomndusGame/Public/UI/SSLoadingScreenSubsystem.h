// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "SSLoadingScreenSubsystem.generated.h"


class UObject;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadingScreenWidgetChangedDelegate, TSubclassOf<UUserWidget>, NewWidgetClass);

/**
 * A subsystem responsible for managing and displaying loading screens within the application.
 *
 * The USSLoadingScreenSubsystem class provides functionality to display and manage loading screens
 * during transitions or resource-loading phases. It allows for the customization and control of
 * loading screen appearance and behavior. This subsystem integrates into the game's architecture and
 * handles operations related to preloading resources or maintaining successful user feedback during
 * loading sequences.
 *
 * Main features of this class include:
 * - Displaying loading screens during transitions or high-load operations.
 * - Providing APIs to customize loading screen visuals and elements.
 * - Handling lifecycle management for activation and deactivation of loading screens.
 * - Allowing loading status updates or progress tracking for visual feedback.
 *
 * This subsystem is particularly useful in managing seamless transition states while communicating
 * progress or processing in an interactive and visually appealing manner.
 *
 * Usage of this class comes as part of the larger application framework and ties into global
 * subsystem management to ensure the loading process is encapsulated and modularized.
 */
UCLASS(Config = Game)
class SOMNDUSGAME_API USSLoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
			
public:
	
	USSLoadingScreenSubsystem();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "LoadingScreen", meta=(WorldContext="WorldContextObject"))
	static void ChangeLoadingScreen(const UObject* WorldContextObject, TSoftClassPtr<UUserWidget> LoadingScreenWidget);
	
	// Sets the loading screen widget class to display inside of the loading screen widget host
	UFUNCTION(BlueprintCallable)
	void SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable)
	void ResetLoadingScreenContentWidget();
	
	// Returns the last set loading screen widget class to display inside of the loading screen widget host
	UFUNCTION(BlueprintPure)
	TSubclassOf<UUserWidget> GetLoadingScreenContentWidget() const;

private:

	UPROPERTY(config)
	TSoftClassPtr<UUserWidget> DefaultLoadingScreenWidgetClass;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

	UPROPERTY(BlueprintAssignable, meta=(AllowPrivateAccess))
	FLoadingScreenWidgetChangedDelegate OnLoadingScreenWidgetChanged;
};
