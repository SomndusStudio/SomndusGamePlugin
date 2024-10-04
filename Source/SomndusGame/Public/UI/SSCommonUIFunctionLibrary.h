/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSCommonUIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SS|UI")
	static void SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|UI", meta = (WorldContext = "WorldContextObject"))
	static ULocalPlayer* GetLocalPlayerFromContext(UObject* WorldContextObject);
};
