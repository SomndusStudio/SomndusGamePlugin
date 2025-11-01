/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSHelperStatics.generated.h"

/**
 * Utility functions for various operations such as gameplay tags, reflection, assets, and data tables.
 *
 * USSHelperStatics provides a collection of static helper methods that can be used
 * for gameplay tag conversions, reflection-based function calling, asset resolution,
 * and manipulation of data tables and arrays.
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

	UFUNCTION(BlueprintCallable, Category = "Reflection")
	static bool CallFunctionByName(UObject* Target, const FString& FunctionName);

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

	template <typename ElementType>
	static void AppendUniqueValue(TArray<ElementType>& Array, const TArray<ElementType>& OtherArray)
	{
		for (const auto& OtherValue : OtherArray)
		{
			if (Array.Contains(OtherValue)) continue;

			Array.AddUnique(OtherValue);
		}
	}

	template <typename ElementType>
	static void UnmergeFromArray(TArray<ElementType>& Array, TArray<ElementType>& OtherArray)
	{
		for (const auto& OtherValue : OtherArray)
		{
			Array.Remove(OtherValue);
		}
	}

	/**
	 * Attempts to retrieve an asset from a TSoftObjectPtr.
	 *
	 * If the asset is already loaded, it is returned directly.
	 * Otherwise, a warning is logged and the asset is loaded synchronously as a fallback.
	 *
	 * @tparam AssetType The expected type of the asset (e.g., USkeletalMesh, UTexture2D).
	 * @param SoftPtr Soft reference to the asset.
	 * @param Context Optional string used to identify the log context (e.g., calling function).
	 * @return Pointer to the loaded asset, or nullptr if the soft pointer is null.
	 */
	template <typename AssetType>
	static AssetType* TryGetAsset(const TSoftObjectPtr<AssetType>& SoftPtr, const FString& Context = "SomndusGame")
	{
		if (SoftPtr.IsNull())
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Soft pointer is null."), *Context);
			return nullptr;
		}

		if (AssetType* Loaded = SoftPtr.Get())
		{
			return Loaded;
		}

		// Asset not loaded: log warning
		UE_LOG(LogTemp, Warning,
		       TEXT("[%s] Asset '%s' was not preloaded asynchronously. Falling back to LoadSynchronous()."),
		       *Context, *SoftPtr.ToString());

		return SoftPtr.LoadSynchronous();
	}

	/**
	 * Attempts to retrieve a class from a TSoftClassPtr.
	 *
	 * If the class is already loaded, it is returned directly.
	 * Otherwise, a warning is logged and the class is loaded synchronously as a fallback.
	 *
	 * @tparam ClassType The expected type of the class (e.g., UUserWidget, UAnimInstance).
	 * @param SoftClassPtr Soft reference to the class.
	 * @param Context Optional string used to identify the log context (e.g., calling function).
	 * @return Pointer to the loaded class, or nullptr if the soft class pointer is null.
	 */
	template <typename ClassType>
	static UClass* TryGetClass(const TSoftClassPtr<ClassType>& SoftClassPtr, const FString& Context = "SomndusGame")
	{
		if (SoftClassPtr.IsNull())
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Soft class pointer is null."), *Context);
			return nullptr;
		}

		if (UClass* LoadedClass = SoftClassPtr.Get())
		{
			return LoadedClass;
		}

		UE_LOG(LogTemp, Warning,
		       TEXT("[%s] Class '%s' was not preloaded asynchronously. Falling back to LoadSynchronous()."),
		       *Context, *SoftClassPtr.ToString());

		return SoftClassPtr.LoadSynchronous();
	}

	/**
	 * Blueprint wrapper of TryGetAsset
	 *
	 * @param SoftObject A soft reference to any UObject-derived asset.
	 * @param Context Optional context string for logging.
	 * @return The resolved UObject, or null if not valid or failed to load.
	 */
	UFUNCTION(BlueprintCallable, Category="SomndusGame|Asset", meta=(DisplayName="TryGetAsset"))
	static UObject* BP_TryGetAsset(const TSoftObjectPtr<UObject>& SoftObject, const FString& Context = TEXT("SomndusGame"));

	/**
	 *  Blueprint wrapper of TryGetClass
	 *
	 * @param SoftClass A soft reference to any UObject-derived asset.
	 * @param Context Optional context string for logging.
	 * @return The resolved UObject, or null if not valid or failed to load.
	 */
	UFUNCTION(BlueprintCallable, Category="SomndusGame|Asset", meta=(DisplayName="TryGetClass"))
	static UClass* BP_TryGetClass(const TSoftClassPtr<UObject>& SoftClass, const FString& Context = TEXT("SomndusGame"));

	/**
	 * Efficiently concatenates multiple FString arguments using variadic templates.
	 *
	 * This function calculates the total length of all input strings to preallocate memory,
	 * minimizing reallocations during concatenation.
	 *
	 * @param First  The first FString to concatenate (required).
	 * @param Rest   Additional FStrings to concatenate (zero or more).
	 * @return       A single FString containing all input strings concatenated in order.
	 */
	template<typename... FStringArgs>
	static FString ConcatStrings(const FString& First, const FStringArgs&... Rest)
	{
		// Calculate total length of all strings for efficient preallocation.
		int32 TotalLen = First.Len() + (Rest.Len() + ... + 0);
	 
		FString Result;
		Result.Empty(TotalLen + 1); // Preallocate memory (+1 for safety)
	 
		Result += First;            // Append the first string
		((Result += Rest), ...);    // Append each additional string
	 
		return Result;
	}
};
