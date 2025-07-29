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

	UFUNCTION(BlueprintPure)
	static int GetPlayInEditorID();

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

	template <class T>
	static TArray<T> GetAllDataTableRow(const UDataTable* InDataTable)
	{
		// name not none
		TArray<T> Results;
		TArray<T*> Rows;
		InDataTable->GetAllRows("All Rows", Rows);

		for (auto Row : Rows)
		{
			Results.Add(*Row);
		}
		return Results;
	}

	template <class T>
	static void ShuffleArray(T& InArray)
	{
		const int32 LastIndex = InArray.Num() - 1;
		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(0, LastIndex);
			if (i != Index)
			{
				InArray.Swap(i, Index);
			}
		}
	}

	template <class T>
	static void ShuffleArray(T& InArray, const FRandomStream& InRandomStream)
	{
		const int32 LastIndex = InArray.Num() - 1;
		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = InRandomStream.RandRange(0, LastIndex);
			if (i != Index)
			{
				InArray.Swap(i, Index);
			}
		}
	}

	template <class T>
	static void ShuffleArray(T& InArray, int32 Seed)
	{
		const FRandomStream Stream{Seed};
		const int32 LastIndex = InArray.Num() - 1;
		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = Stream.RandRange(0, LastIndex);
			if (i != Index)
			{
				InArray.Swap(i, Index);
			}
		}
	}
};
