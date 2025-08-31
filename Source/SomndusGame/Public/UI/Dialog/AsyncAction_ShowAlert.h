/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Messaging/CommonGameDialog.h"
#include "UI/SSCommonUITypes.h"
#include "AsyncAction_ShowAlert.generated.h"

enum class ECommonMessagingResult : uint8;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API UAsyncAction_ShowAlert : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAsyncAction_ShowAlert* ShowAlert(
		UObject* InWorldContextObject, FText Title, FText Message
	);
	
	virtual void Activate() override;

public:
	UPROPERTY(BlueprintAssignable)
	FSSCommonMessagingResultMCDelegate OnResult;

private:
	void HandleAlertResult(ECommonMessagingResult AlertResult);

	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> TargetLocalPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UCommonGameDialogDescriptor> Descriptor;
};
