// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "Actions/AsyncAction_PushContentToLayerForPlayer.h"
#include "Engine/CancellableAsyncAction.h"
#include "SSAsyncAction_PushContentForLocal.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSAsyncAction_PushContentForLocal : public UCancellableAsyncAction
{
	GENERATED_BODY()
	
public:
	virtual void Cancel() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static USSAsyncAction_PushContentForLocal* PushContentToLayerForLocal(UObject* WorldContextObject, UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass, FGameplayTag LayerName, bool bSuspendInputUntilComplete = true);

	virtual void Activate() override;

public:

	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerForPlayerAsyncDelegate BeforePush;

	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerForPlayerAsyncDelegate AfterPush;

private:

	FGameplayTag LayerName;
	bool bSuspendInputUntilComplete = false;
	TWeakObjectPtr<APlayerController> OwningPlayerPtr;
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

	TSharedPtr<FStreamableHandle> StreamingHandle;
};
