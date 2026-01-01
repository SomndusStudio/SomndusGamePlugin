/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Notification/SSGameNotificationManager.h"

#include "SSLog.h"
#include "UI/SSGameMessagingSubsystem.h"

USSGameNotificationManager* USSGameNotificationManager::Get(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: WorldContextObject is null."));
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: could not retrieve UWorld from context."));
		return nullptr;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: no PlayerController found."));
		return nullptr;
	}

	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: no LocalPlayer found on PlayerController."));
		return nullptr;
	}

	USSGameMessagingSubsystem* Subsystem = LP->GetSubsystem<USSGameMessagingSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: USSGameMessagingSubsystem not found."));
		return nullptr;
	}

	USSGameNotificationManager* Manager = Subsystem->GetNotificationManager();
	if (!Manager)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("Get() failed: NotificationManager was null in MessagingSubsystem."));
		return nullptr;
	}

	return Manager;
}

void USSGameNotificationManager::AddNotification(const FSSGameNotificationInfo& Info)
{
	if (Info.bAvoidDuplicates && Info.Identifier != NAME_None)
	{
		if (ActiveNotificationIdentifiers.Contains(Info.Identifier))
		{
			return;
		}

		ActiveNotificationIdentifiers.Add(Info.Identifier);
	}

	OnNotificationRequested.Broadcast(Info);
}
