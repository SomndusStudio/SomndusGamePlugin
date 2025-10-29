// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSInputObjectBase.generated.h"

class ACharacter;
class AController;
class USSInputContextComponent;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputObjectBase : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSInputContextComponent> InputComponent;

	UFUNCTION(BlueprintPure)
	AController* GetController();
	
	UFUNCTION(BlueprintPure)
	APlayerController* GetPlayerController();
	
	UFUNCTION(BlueprintPure)
	APawn* GetControlledPawn();

	UFUNCTION(BlueprintCallable)
	ACharacter* GetControlledCharacter();
	
	virtual USSInputContextComponent* GetNativeInputComponent();
	
	UFUNCTION(BlueprintPure)
	USSInputContextComponent* GetInputComponent();
};
