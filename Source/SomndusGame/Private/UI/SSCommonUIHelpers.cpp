/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonUIHelpers.h"
#include "Engine/Engine.h"

TSharedPtr<FStreamableHandle> USSCommonUIHelpers::PushWidgetWithAllStatesCallback(
	APlayerController* PlayerController,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
	FGameplayTag LayerName,
	bool bSuspendInputUntilComplete,
	FWidgetLayerAsyncCallback Callback
)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PushWidgetWithAllStatesCallback: PlayerController is null."));
		return nullptr;
	}
	if (WidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("PushWidgetWithAllStatesCallback: WidgetClass is null."));
		return nullptr;
	}

	UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(PlayerController);
	if (!RootLayout)
	{
		UE_LOG(LogTemp, Error, TEXT("PushWidgetWithAllStatesCallback: PrimaryGameLayout not found."));
		return nullptr;
	}

	// Appel de PushWidgetToLayerStackAsync avec callback inline qui transmet l'état au Callback utilisateur
	return RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(
		LayerName,
		bSuspendInputUntilComplete,
		WidgetClass,
		[Callback](EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
		{
			if (Callback)
			{
				Callback(State, Widget);
			}
		}
	);
}