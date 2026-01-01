// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSContextObject.h"
#include "SSGameNotificationManager.generated.h"

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSGameNotificationInfo
{
	GENERATED_BODY()

	/** Optional identifier to avoid duplicate notifications */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	FName Identifier;

	/** Prevent duplicate notifications with same Identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	bool bAvoidDuplicates = false;

	/** Duration to display the notification (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	float Duration = 3.f;

	/** Notification type, like Success, Error, Info... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	FGameplayTag Type;

	/** Notification title or main text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	FText Title;

	/** Optional description or body */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	FText Message;

	/** Optional payload for user-specific data (can be expanded later) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notification")
	UObject* Payload = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSGameNotificationEvent, const FSSGameNotificationInfo&, Info);

/**
 * USSGameNotificationManager
 *
 * Singleton manager responsible for handling game notifications and routing them to UI widgets.
 * Provides global access to add notifications and broadcasts events when notifications are requested.
 */
UCLASS()
class SOMNDUSGAME_API USSGameNotificationManager : public USSContextObject
{
	GENERATED_BODY()

public:
	/** Returns the global instance of the notification manager */
	UFUNCTION(BlueprintCallable, Category="Notification", meta=(WorldContext="WorldContextObject"))
	static USSGameNotificationManager* Get(UObject* WorldContextObject);

	/** Adds a new notification, routed to the UI widget (BP-implemented) */
	UFUNCTION(BlueprintCallable, Category="Notification")
	void AddNotification(const FSSGameNotificationInfo& Info);

	/** Broadcast when a notification is added */
	UPROPERTY(BlueprintAssignable, Category = "Notification")
	FSSGameNotificationEvent OnNotificationRequested;

protected:
	/** Active notifications currently displayed (used to prevent duplicates) */
	UPROPERTY(Transient)
	TArray<FName> ActiveNotificationIdentifiers;
};
