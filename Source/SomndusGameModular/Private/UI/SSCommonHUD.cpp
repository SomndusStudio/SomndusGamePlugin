// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SSCommonHUD.h"

#include "SSGameModularTypes.h"
#include "Components/GameFrameworkComponentManager.h"

static TAutoConsoleVariable<bool> DUHUDShow(TEXT("Somndus.HUD.Show"), true, TEXT("Show Whole Player HUD ?"));

FName ASSCommonHUD::NAME_HUDReady = FName("HUDReady");

ASSCommonHUD::ASSCommonHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASSCommonHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASSCommonHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	
	Super::BeginPlay();

	// Listen command change
	DUHUDShow->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &ASSCommonHUD::OnShowHUDChange));

	SSGameModularHelpers::SendGameFrameworkComponentExtensionEvent(GetOwner(), ASSCommonHUD::NAME_HUDReady);
}

void ASSCommonHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}

void ASSCommonHUD::OnShowHUDChange(IConsoleVariable* ConsoleVariable)
{
	bShowHUD = ConsoleVariable->GetInt() != 0;
}
