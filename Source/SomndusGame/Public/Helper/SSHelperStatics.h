/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSHelperStatics.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSHelperStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="SomndusStudio|Helper|String")
	static TArray<FName> ConvertGTagsToFTags(const TArray<FGameplayTag>& InGameplayTags);
	
	template <class T>
	static bool GetDataTableRow(const UDataTable* InDataTable, FName Identifier, T& OutData)
	{
		// name not none
		if (!Identifier.IsNone())
		{
			const T* Item = InDataTable->FindRow<T>(Identifier, "Get Datatable Row Data");
			if (Item != nullptr)
			{
				OutData = *Item;
				return true;
			}
			// Not valid
			return false;
		}
		return false;
	}

	template <class T>
	static bool GetDataTableRowWithDefault(const UDataTable* InDataTable, FName Identifier, FName Default, T& OutData)
	{
		if (!GetDataTableRow<T>(InDataTable, Identifier, OutData))
		{
			return GetDataTableRow<T>(InDataTable, Default, OutData);
		}
		return true;
	}

	template <class T>
	static bool GetDataTableRowFromInt(const UDataTable* InDataTable, const int32 Identifier, T& OutData)
	{
		return GetDataTableRow(InDataTable, FName(FString::FromInt(Identifier)), OutData);
	}
};
