/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Task/SSGameTaskBase.h"

#include "TimerManager.h"
#include "Engine/TimerHandle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void USSGameTaskBase::OnStart_Implementation()
{
}

void USSGameTaskBase::OnFinish_Implementation()
{
}

void USSGameTaskBase::InternalRequestStart()
{
	NativeOnPreStart();
	
	if (FakeDelay <= 0.0f)
	{
		NativeOnStart();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("USSGameTaskBase: Fake delay of %f seconds before starting task."), FakeDelay);
	 
		GetWorld()->GetTimerManager().SetTimer(
			PreStartTimerHandle, this, &USSGameTaskBase::OnPreStartEnd, FakeDelay, false);
	}
}

void USSGameTaskBase::OnPreStartEnd()
{
	NativeOnStart();
}

void USSGameTaskBase::NativeOnPreStart()
{
	UE_LOG(LogTemp, Log, TEXT("USSGameTaskBase::NativeOnPreStart"));
}

void USSGameTaskBase::NativeOnStart()
{
	UE_LOG(LogTemp, Log, TEXT("USSGameTaskBase::NativeOnStart"));
	
	bRunning = true;
	
	GetWorld()->GetTimerManager().SetTimer(MaxTimeoutHandle, this, &USSGameTaskBase::TimerElapsed, GameTaskSetting.MaxTimeout, false);
	
	OnStart();
}

void USSGameTaskBase::PerformFinish(ESSGameTaskResult InResult)
{
	bRunning = false;
	Result = InResult;
	
	OnFinish();

	OnFinishSignature.Broadcast(this, Result);
}

void USSGameTaskBase::TimerElapsed()
{
	// If not finished
	if (bRunning)
	{
		PerformFinish(ESSGameTaskResult::Timeout);
	}
}

ULocalPlayer* USSGameTaskBase::GetOwningLocalPlayer() const
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return nullptr;
	return PlayerController->GetLocalPlayer();
}
