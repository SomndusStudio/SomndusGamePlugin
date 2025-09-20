// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "SSGameViewportClient.generated.h"

class UGameInstance;
class UObject;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	USSGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
