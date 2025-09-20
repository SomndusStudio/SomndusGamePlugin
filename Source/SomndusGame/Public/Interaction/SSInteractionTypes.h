// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/PlayerInput.h"
#include "UObject/Object.h"
#include "SSInteractionTypes.generated.h"

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInteractionPrompt : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 InputId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class UInputAction> EnhancedInputAction = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Priority = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInteractionContext : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSSInteractionPrompt Prompt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* Object = nullptr;

	bool Equals(const FSSInteractionContext& Other) const
	{
		return Prompt.InputId == Other.Prompt.InputId || Prompt.EnhancedInputAction == Other.Prompt.EnhancedInputAction;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSInteractionEvent, FSSInteractionContext, InteractionContext);