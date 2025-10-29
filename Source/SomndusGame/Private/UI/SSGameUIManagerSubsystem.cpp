/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSGameUIManagerSubsystem.h"

#include "CommonLocalPlayer.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Fade/SSScreenFadeWidget.h"
#include "GameFramework/HUD.h"
#include "UI/Transition/SSWidgetTransitionMgr.h"

void USSGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TooltipManager = NewObject<USSTooltipManager>(this);
	CursorManager = NewObject<USSCursorManager>(this);
	CursorManager->Init(this);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &USSGameUIManagerSubsystem::Tick), 0.0f);
}

void USSGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

bool USSGameUIManagerSubsystem::Tick(float DeltaTime)
{
	SyncRootLayoutVisibilityToShowHUD();

	TooltipManager->Tick(DeltaTime);
	
	return true;
}

void USSGameUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD()
{
	if (const UGameUIPolicy* Policy = GetCurrentUIPolicy())
	{
		for (const ULocalPlayer* LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			bool bShouldShowUI = true;
			
			if (const APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
			{
				const AHUD* HUD = PC->GetHUD();

				if (HUD && !HUD->bShowHUD)
				{
					bShouldShowUI = false;
				}
			}

			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				const ESlateVisibility DesiredVisibility = bShouldShowUI ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
				if (DesiredVisibility != RootLayout->GetVisibility())
				{
					RootLayout->SetVisibility(DesiredVisibility);	
				}
			}
		}
	}
}

void USSGameUIManagerSubsystem::AddFadeWidget(const FSSScreenFadeParams& FadeParams, const APlayerController* OwningPlayer, const int32 ZOrder)
{
	const int32 ControllerID = GetPlayerControllerID(OwningPlayer);
	
	if (FadeWidgetsForID.Contains(ControllerID))
	{
		RemoveFadeWidget(OwningPlayer, ControllerID);
	}

	FSSScreenFadeDelegate OnFadeFinished;

	if (FadeParams.ToColor.A <= 0.0f)
	{
		OnFadeFinished.BindUObject(this, &ThisClass::RemoveFadeWidget, ControllerID);
	}

	TSharedRef<SSSScreenFadeWidget> FadeWidget = SNew(SSSScreenFadeWidget).FadeParams(FadeParams).OnFadeFinished(OnFadeFinished);

	if (UGameViewportClient* GameViewport = GetGameViewport())
	{
		if (OwningPlayer)
		{
			GameViewport->AddViewportWidgetForPlayer(OwningPlayer->GetLocalPlayer(), FadeWidget, ZOrder);
		}
		else
		{
			GameViewport->AddViewportWidgetContent(FadeWidget, ZOrder + 10);
		}
	}

	FadeWidgetsForID.Emplace(ControllerID, FadeWidget);
	FadeWidget->StartFade();
}

void USSGameUIManagerSubsystem::RemoveFadeWidget(const APlayerController* OwningPlayer, const int32 ControllerID)
{
	TSharedRef<SWidget> FadeWidget = FadeWidgetsForID[ControllerID].Pin().ToSharedRef();
	FadeWidgetsForID.Remove(ControllerID);

	if (UGameViewportClient* GameViewport = GetGameViewport())
	{
		if (OwningPlayer)
		{
			GameViewport->RemoveViewportWidgetForPlayer(OwningPlayer->GetLocalPlayer(), FadeWidget);
		}
		else
		{
			GameViewport->RemoveViewportWidgetContent(FadeWidget);
		}
	}
}

void USSGameUIManagerSubsystem::RemoveFadeWidget(const int32 ControllerID)
{
	RemoveFadeWidget(GetPlayerControllerFromID(ControllerID), ControllerID);
}

UGameViewportClient* USSGameUIManagerSubsystem::GetGameViewport() const
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameViewportClient* GameViewport = World->GetGameViewport())
		{
			return GameViewport;
		}
	}

	return nullptr;
}

int32 USSGameUIManagerSubsystem::GetPlayerControllerID(const APlayerController* PlayerController) const
{
	if (PlayerController)
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			return LocalPlayer->GetControllerId();
		}
	}

	return 0x80000000;
}

APlayerController* USSGameUIManagerSubsystem::GetPlayerControllerFromID(const int32 ControllerID) const
{
	if (ControllerID == 0x80000000)
	{
		return nullptr;
	}

	if (const UWorld* World = GetWorld())
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();

			if (GetPlayerControllerID(PlayerController) == ControllerID)
			{
				return PlayerController;
			}
		}
	}

	return nullptr;
}

USSWidgetTransitionMgr* USSGameUIManagerSubsystem::GetWidgetTransitionMgr()
{
	if (!WidgetTransitionMgr)
	{
		WidgetTransitionMgr = NewObject<USSWidgetTransitionMgr>(this);
	}
	return WidgetTransitionMgr;
}
