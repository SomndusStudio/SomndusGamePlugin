/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "SSInputLocalPlayerSubsystem.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputMappingContextPullToken
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const UInputMappingContext* MappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Token;
};

/**
 * 
 */
UCLASS(config = Game)
class SOMNDUSGAME_API USSInputLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	/**
	 * Allows identifying which generated token is responsible for suspending player inputs during a UI animation
	 */
	FName CommonUIAnimationInputSuspendToken;

	/**
	 * Allows identifying which generated token is responsible for suspending player inputs during a Gameplay
	 */
	FName CommonGameplayInputSuspendToken;

	UPROPERTY()
	TMap<FGameplayTag, FName> GameSuspendTokens;
	
public:
	/**
	 * Cached version of all mapping key mapping know
	 * used because the native enhanced input don't get key are not in active mapping context
	 */
	UPROPERTY()
	TArray<FEnhancedActionKeyMapping> KeyMappingRows;

	void UpdateKeyMappingCache(const UEnhancedInputLocalPlayerSubsystem* EISubsystem);
	void UpdateKeyMappingCache();
	
	UPROPERTY()
	TArray<FSSInputMappingContextPullToken> InputMappingContextPullTokens;

	bool FirstTimeMappingContext(const UInputMappingContext* MappingContext);
	bool ContainsMappingContextPull(const UInputMappingContext* MappingContext);

	bool PullContainsMappingContext(const UInputMappingContext* MappingContext, FString Token);

	void AddPullMappingContext(const UInputMappingContext* MappingContext, FString Token);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PullMappingContext(const UInputMappingContext* MappingContext, int32 InputMappingPriority, FString Token);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PullGlobalMappingContext(FString Token);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PopGlobalMappingContext(FString Token);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PopMappingContext(const UInputMappingContext* MappingContext, FString Token);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SuspendInputForUIAnimation();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void ResumeInputForUIAnimation();
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SuspendInputForGameplay();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void ResumeInputForGameplay();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void SuspendInputForTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void ResumeInputForTag(FGameplayTag Tag);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> GenericUIMappingContext;
	
	/**
	 * To use in every activatable widget instead specific one by one
	 */
	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UInputMappingContext> GenericUIMappingContextSoft;
};
