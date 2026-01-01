/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSContextObject.h"
#include "SSInputContextDefinition.h"
#include "SSInputHandlerContext.h"
#include "SSPawnInputContext.generated.h"

class USSInputContextComponent;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSPawnInputContext : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly)
	APawn* Pawn;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSInputContextComponent> InputContextComponent;

	void NativeInitialize(const TObjectPtr<USSInputContextComponent>& InInputContextComponent, const FGameplayTag& InContextTag);
	
	// Activates the appropriate input handler based on internal priorities or gameplay tags
	UFUNCTION(BlueprintCallable, Category="Input")
	void ActivateInputHandlers(UEnhancedInputComponent* InputComp);

	void Deactivate(UEnhancedInputComponent* InputComp);
	
	// Add handler
	UFUNCTION(BlueprintCallable, Category="Input")
	void RegisterHandler(const FGameplayTag& InTag, USSInputHandlerContext* Handler);

	virtual void SetPawn(APawn* InPawn);
	
	UFUNCTION(BlueprintPure)
	APawn* GetPawn();
	
	USSInputContextComponent* GetInputContextComponent();

	USSInputHandlerContext* GetOrCreateInputHandlerContext(const FGameplayTag& Tag, const TSubclassOf<USSInputHandlerContext>& InClass = USSInputHandlerContext::StaticClass());
	
	USSInputHandlerContext* PushInputContextDefinition(USSInputContextDefinition* InputContextDefinition);
	
	UFUNCTION(BlueprintCallable)
	bool SetActiveInputContextFromTag(const FGameplayTag& InContextTag, bool bActive);
	
protected:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USSInputHandlerContext>> InputHandlerContexts;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<USSInputHandlerContext>> ActiveInputHandlerContexts;
	
	UPROPERTY()
	TObjectPtr<USSInputHandlerContext> ActiveHandler;

public:

	USSInputHandlerContext* GetActiveHandler() { return ActiveHandler; }
	
	TArray<TObjectPtr<USSInputHandlerContext>> GetActiveInputHandlerContexts();

	template<typename T>
	T* FindHandlerByClass() const
	{

		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must derive from UObject");

		for (const TObjectPtr<USSInputHandlerContext>& Handler : ActiveInputHandlerContexts)
		{
			if (T* TypedHandler = Cast<T>(Handler))
			{
				return TypedHandler;
			}
		}

		return nullptr;
	}
};
