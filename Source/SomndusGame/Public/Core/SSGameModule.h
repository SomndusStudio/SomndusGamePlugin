// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSGameModule.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameModule : public USSContextObject
{
	GENERATED_BODY()

public:
	
	virtual void NativeInit();
};
