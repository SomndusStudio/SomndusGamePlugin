/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputMappingContext.h"
#include "SSInputLocalPlayerSubsystem.generated.h"

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
	
	UPROPERTY()
	TArray<FSSInputMappingContextPullToken> InputMappingContextPullTokens;

	bool FirstTimeMappingContext(const UInputMappingContext* MappingContext);
	bool ContainsMappingContextPull(const UInputMappingContext* MappingContext);

	bool PullContainsMappingContext(const UInputMappingContext* MappingContext, FString Token);

	void AddPullMappingContext(const UInputMappingContext* MappingContext, FString Token);
	
	UFUNCTION(BlueprintCallable)
	void PullMappingContext(const UInputMappingContext* MappingContext, int32 InputMappingPriority, FString Token);

	UFUNCTION(BlueprintCallable)
	void PullGlobalMappingContext(FString Token);

	UFUNCTION(BlueprintCallable)
	void PopGlobalMappingContext(FString Token);
	
	UFUNCTION(BlueprintCallable)
	void PopMappingContext(const UInputMappingContext* MappingContext, FString Token);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> GenericUIMappingContext;
	
	/**
	 * To use in every activatable widget instead specific one by one
	 */
	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UInputMappingContext> GenericUIMappingContextSoft;
};
