/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSGameTaskBase.generated.h"

class ULocalPlayer;
// Show Modal Type
UENUM(BlueprintType)
enum class ESSGameTaskResult : uint8
{
	Unknown,
	Success,
	Cancelled,
	Error,
	Timeout,
	Finished
};

/**
* DELEGATES
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSGameTaskResultSignature, class USSGameTaskBase*, GameTask, ESSGameTaskResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSGameTaskSignature, class USSGameTaskBase*, GameTask);

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSGameTaskSetting
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTimeout = 14.0f;
};

/**
 * Base class for creating game tasks in the SS system.
 * Defines the foundational structure and functionality that all game tasks should inherit and extend.
 */
UCLASS()
class SOMNDUSGAME_API USSGameTaskBase : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESSGameTaskResult Result;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSGameTaskSetting GameTaskSetting;

	virtual void NativeOnStart();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFinish();
	
	UFUNCTION(BlueprintCallable)
	void PerformFinish(ESSGameTaskResult InResult=ESSGameTaskResult::Success);

	UFUNCTION()
	void TimerElapsed();

	UPROPERTY()
	const UObject* WorldContextObject;
	
	UPROPERTY(BlueprintAssignable)
	FSSGameTaskResultSignature OnFinishSignature;

protected:
	
	ULocalPlayer* GetOwningLocalPlayer() const;
	
};
