/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "Containers/Ticker.h"
#include "Cursor/SSCursorManager.h"
#include "Fade/SSScreenFadeTypes.h"
#include "Tooltip/SSTooltipManager.h"
#include "SSGameUIManagerSubsystem.generated.h"

class USSWidgetTransitionMgr;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	FTSTicker::FDelegateHandle TickHandle;

public:
	void AddFadeWidget(const FSSScreenFadeParams& FadeParams, const APlayerController* OwningPlayer = nullptr, const int32 ZOrder = 100);

private:
	void RemoveFadeWidget(const APlayerController* OwningPlayer, const int32 ControllerID);
	void RemoveFadeWidget(const int32 ControllerID);

	UGameViewportClient* GetGameViewport() const;
	int32 GetPlayerControllerID(const APlayerController* PlayerController) const;
	APlayerController* GetPlayerControllerFromID(const int32 ControllerID) const;

	TMap<int32, TWeakPtr<SWidget>> FadeWidgetsForID;
	
public:
	UPROPERTY()
	TObjectPtr<USSTooltipManager> TooltipManager;
	
	UPROPERTY()
	TObjectPtr<USSCursorManager> CursorManager;

	UPROPERTY()
	TObjectPtr<USSWidgetTransitionMgr> WidgetTransitionMgr;

	UFUNCTION(BlueprintCallable)
	USSWidgetTransitionMgr* GetWidgetTransitionMgr();

	UFUNCTION(BlueprintCallable)
	USSTooltipManager* GetTooltipManager() const { return TooltipManager; }
};
