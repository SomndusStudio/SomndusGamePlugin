/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSGameTaskBase.h"
#include "Engine/CancellableAsyncAction.h"
#include "SSGameTaskAsyncAction.generated.h"

/**
 * Represents an asynchronous action within the USS game task system.
 *
 * This class provides the framework for executing game tasks asynchronously.
 * It serves as a base or utility class for handling asynchronous operations
 * specific to the gameplay tasks within the USS system.
 */
UCLASS()
class SOMNDUSGAME_API USSGameTaskAsyncAction : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSSGameTaskResultSignature OnInit;

	UPROPERTY(BlueprintAssignable)
	FSSGameTaskResultSignature OnStart;
	
	UPROPERTY(BlueprintAssignable)
	FSSGameTaskResultSignature OnFinish;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
	Category = "Gameplay")
	static USSGameTaskAsyncAction* AsyncGameTask(const UObject* WorldContextObject, TSoftClassPtr<USSGameTaskBase> GameTaskClass, FSSGameTaskSetting GameTaskSetting);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface
	
	UFUNCTION()
	void OnTaskFinish(USSGameTaskBase* GameTask, ESSGameTaskResult InResult);
	
	UFUNCTION()
	void OnLoadComplete();
	
	// Allows the Object to use BP_Functions
	virtual class UWorld* GetWorld() const override;

private:
	const UObject* WorldContextObject;
	
	/**
	 * Modal widget want to spawn
	 */
	UPROPERTY()
	TSoftClassPtr<USSGameTaskBase> GameTaskClass;
	
	UPROPERTY()
	USSGameTaskBase* GameTaskObject;
	
	UPROPERTY()
	FSSGameTaskSetting GameTaskSetting;
};

