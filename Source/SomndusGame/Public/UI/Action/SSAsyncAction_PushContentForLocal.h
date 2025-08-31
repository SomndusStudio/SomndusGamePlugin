/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Actions/AsyncAction_PushContentToLayerForPlayer.h"
#include "Engine/CancellableAsyncAction.h"
#include "SSAsyncAction_PushContentForLocal.generated.h"

/**
 * Asynchronous Blueprint node for pushing a Common UI widget to a specific layer for a local player.
 * 
 * This async action is used to push a widget (derived from UCommonActivatableWidget) onto a named UI layer 
 * for a local player. It exposes delegate events for customization before and after the push operation, 
 * and allows optional input suspension until the operation completes.
 *
 * Usage:
 * - Call PushContentToLayerForLocal() to start the async flow.
 * - Bind to BeforePush and AfterPush delegates to react to the process stages.
 * - Call Cancel() to cancel the operation if needed.
 */
UCLASS()
class SOMNDUSGAME_API USSAsyncAction_PushContentForLocal : public UCancellableAsyncAction
{
	GENERATED_BODY()
	
public:
	virtual void Cancel() override;

	/**
	 * Begins the async operation to push a widget to the specified UI layer for the local player.
	 * 
	 * @param WorldContextObject        Required for world context in Blueprint.
	 * @param WidgetClass               A soft class pointer to the widget to push. Must derive from UCommonActivatableWidget.
	 * @param LayerName                 Tag identifying the target UI layer to push the widget to.
	 * @param bSuspendInputUntilComplete If true, input will be suspended until the operation is complete.
	 * 
	 * @return A new instance of USSAsyncAction_PushContentForLocal to bind events to and control the async flow.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static USSAsyncAction_PushContentForLocal* PushContentToLayerForLocal(UObject* WorldContextObject, UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass, FGameplayTag LayerName, bool bSuspendInputUntilComplete = true);

	/** Starts the operation when the async action is activated (internal use). */
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
