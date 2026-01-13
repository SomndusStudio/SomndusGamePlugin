// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSLoadingScreenSubsystem.h"

#include "Engine/GameInstance.h"
#include "Helper/SSHelperStatics.h"
#include "Kismet/GameplayStatics.h"


USSLoadingScreenSubsystem::USSLoadingScreenSubsystem()
{
}

void USSLoadingScreenSubsystem::ChangeLoadingScreen(const UObject* WorldContextObject, TSoftClassPtr<UUserWidget> LoadingScreenWidget)
{
	auto* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	check(GameInstance);

	// Change loading screen if necessary
	auto* LoadingScreenSubsystem = GameInstance->GetSubsystem<USSLoadingScreenSubsystem>();
	check(LoadingScreenSubsystem);

	// TODO : Find a way to load async properly
	if (!LoadingScreenWidget.IsNull())
	{
		auto LoadingScreenWidgetClass = USSHelperStatics::TryGetClass(LoadingScreenWidget);
		LoadingScreenSubsystem->SetLoadingScreenContentWidget(LoadingScreenWidgetClass);
	}
}


void USSLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

void USSLoadingScreenSubsystem::ResetLoadingScreenContentWidget()
{
	auto WidgetClass = USSHelperStatics::TryGetClass(DefaultLoadingScreenWidgetClass);
	SetLoadingScreenContentWidget(WidgetClass);
}

TSubclassOf<UUserWidget> USSLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}
