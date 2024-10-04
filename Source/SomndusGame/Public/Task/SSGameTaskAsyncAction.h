/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSGameTaskBase.h"
#include "Engine/CancellableAsyncAction.h"
#include "SSGameTaskAsyncAction.generated.h"

/**
 * 
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
	 * @brief Modal widget want to spawn
	 */
	UPROPERTY()
	TSoftClassPtr<USSGameTaskBase> GameTaskClass;
	
	UPROPERTY()
	USSGameTaskBase* GameTaskObject;
	
	UPROPERTY()
	FSSGameTaskSetting GameTaskSetting;
};

