/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "Cursor/SSCursorManager.h"
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
	UPROPERTY()
	TObjectPtr<USSTooltipManager> TooltipManager;
	
	UPROPERTY()
	TObjectPtr<USSCursorManager> CursorManager;

	UPROPERTY()
	TObjectPtr<USSWidgetTransitionMgr> WidgetTransitionMgr;

	UFUNCTION(BlueprintCallable)
	USSWidgetTransitionMgr* GetWidgetTransitionMgr();
};
