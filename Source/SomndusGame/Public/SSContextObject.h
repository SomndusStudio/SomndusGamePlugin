/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSContextObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSContextObject : public UObject
{
	GENERATED_BODY()
	virtual void PostLoad() override;

	class ULevel* GetLevel() const;

public:
	// Allows the Object to use BP_Functions
	virtual class UWorld* GetWorld() const override;


	UFUNCTION(BlueprintPure)
	class UGameInstance* GetGameInstance() const;
	
};
