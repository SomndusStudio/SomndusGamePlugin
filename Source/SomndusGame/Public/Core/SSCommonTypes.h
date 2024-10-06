/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSCommonTypes.generated.h"

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSCommonIdInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayPriority=0))
	int32 Id = -1;

	bool IsValid() const
	{
		return Id > 0;
	}
};