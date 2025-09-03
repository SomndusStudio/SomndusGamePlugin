/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Task/SSGameTaskBase.h"

#include "TimerManager.h"
#include "Engine/TimerHandle.h"
#include "Engine/World.h"

void USSGameTaskBase::OnStart_Implementation()
{
	
}

void USSGameTaskBase::OnFinish_Implementation()
{
}

void USSGameTaskBase::NativeOnStart()
{
	bRunning = true;
	
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(
		UnusedHandle, this, &USSGameTaskBase::TimerElapsed, GameTaskSetting.MaxTimeout, false);
	
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
