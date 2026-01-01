/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "SSGameViewportClient.generated.h"

class UGameInstance;
class UObject;

/**
 * USSGameViewportClient is a custom game viewport client derived from UCommonGameViewportClient.
 * This class provides additional initialization logic specific to the SOMNDUSGAME project.
 */
UCLASS()
class SOMNDUSGAME_API USSGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	USSGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
