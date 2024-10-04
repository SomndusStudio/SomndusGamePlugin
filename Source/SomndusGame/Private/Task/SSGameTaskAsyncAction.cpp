/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Task/SSGameTaskAsyncAction.h"

USSGameTaskAsyncAction* USSGameTaskAsyncAction::AsyncGameTask(const UObject* WorldContextObject,
																		  TSoftClassPtr<USSGameTaskBase> GameTaskClass,
																		  FSSGameTaskSetting GameTaskSetting)
{
	USSGameTaskAsyncAction* BlueprintNode = NewObject<USSGameTaskAsyncAction>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->GameTaskClass = GameTaskClass;
	BlueprintNode->GameTaskSetting = GameTaskSetting;
	// Return
	return BlueprintNode;
}

UWorld* USSGameTaskAsyncAction::GetWorld() const
{
	return WorldContextObject->GetWorld();
}

void USSGameTaskAsyncAction::Activate()
{
	Super::Activate();

	// TODO :: Later maybe try to load the soft class async instead LoadSynchronous
	OnLoadComplete();
}

void USSGameTaskAsyncAction::OnTaskFinish(USSGameTaskBase* GameTask, ESSGameTaskResult InResult)
{
	OnFinish.Broadcast(GameTask, InResult);

	SetReadyToDestroy();
}


void USSGameTaskAsyncAction::OnLoadComplete()
{
	// Use the loaded asset
	GameTaskObject = NewObject<USSGameTaskBase>(this, GameTaskClass.LoadSynchronous());
	GameTaskObject->WorldContextObject = WorldContextObject;
	GameTaskObject->GameTaskSetting = GameTaskSetting;
	// Bind
	GameTaskObject->OnFinishSignature.AddDynamic(this, &USSGameTaskAsyncAction::OnTaskFinish);

	// Before Start
	OnInit.Broadcast(GameTaskObject, ESSGameTaskResult::Unknown);
	
	// Start
	GameTaskObject->NativeOnStart();
	OnStart.Broadcast(GameTaskObject, ESSGameTaskResult::Unknown);
}
