/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
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

namespace SSOperationCode
{
	SOMNDUSGAME_API extern const FString CD_SUCCESS;
	SOMNDUSGAME_API extern const FString CD_ERROR;
}

/**
 * Compile-time check to determine whether a given type is a valid USTRUCT().
 *
 * Usage:
 *    static_assert(TIsUStruct<FMyStruct>::Value, "T must be a USTRUCT");
 */
template<typename T>
struct TSSIsUStruct
{
private:
	// Selected if TBaseStructure<U>::Get() is a valid expression (i.e., U is a USTRUCT)
	template<typename U>
	static auto Test(int) -> decltype(TBaseStructure<U>::Get(), std::true_type());

	// Fallback if the above is not valid
	template<typename>
	static std::false_type Test(...);

public:
	// true if the first overload is selected, false otherwise
	static constexpr bool Value = decltype(Test<T>(0))::value;
};

USTRUCT(BlueprintType)
struct FSSOperationResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInstancedStruct Data;
};

/**
 * TSSOperationResult
 *
 * A lightweight templated operation result structure used to represent success or error states
 * alongside structured data of type `T`. Intended for use in C++ only (not exposed to Blueprints).
 *
 * @tparam T Struct type used as the payload for the operation result.
 */
template<typename T>
struct SOMNDUSGAME_API TSSOperationResult
{
	/** Operation result code (e.g., "SUCCESS", "ERROR", etc.). */
	FString Code;

	/** Optional human-readable message describing the result. */
	FText Message;

	/** Payload data of the operation, of arbitrary type `T`. */
	T Data;

	/**
	 * Sets the data payload and returns a reference to the result for chaining.
	 *
	 * @param InData The data to store in the result.
	 * @return Reference to self.
	 */
	TSSOperationResult& SetData(const T& InData)
	{
		Data = InData;
		return *this;
	}

	/** @return Const reference to the data payload. */
	const T& GetData() const { return Data; }

	/** @return Mutable reference to the data payload. */
	T& GetData() { return Data; }

	/** @return True if the operation was a success (based on `Code`). */
	bool IsSuccess() const { return Code == SSOperationCode::CD_SUCCESS; }

	/** @return True if the operation was an error (based on `Code`). */
	bool IsError() const { return Code == SSOperationCode::CD_ERROR; }

	/**
	 * Creates a successful result without a data payload.
	 *
	 * @return A new success result with an empty message and default data.
	 */
	static TSSOperationResult<T> Success()
	{
		TSSOperationResult<T> Result;
		Result.Code = SSOperationCode::CD_SUCCESS;
		return Result;
	}

	/**
	 * Creates an error result without a data payload.
	 *
	 * @return A new error result with an empty message and default data.
	 */
	static TSSOperationResult<T> Error()
	{
		TSSOperationResult<T> Result;
		Result.Code = SSOperationCode::CD_ERROR;
		return Result;
	}

	/**
	 * Marks the result as a success and optionally sets a message.
	 *
	 * @param Reason Optional message to describe the success.
	 */
	void SetSuccess(const FText& Reason = FText::FromString(TEXT("Common Success !")))
	{
		Code = SSOperationCode::CD_SUCCESS;
		Message = Reason;
	}

	/**
	 * Marks the result as an error and optionally sets a message.
	 *
	 * @param Reason Optional message to describe the error.
	 */
	void SetError(const FText& Reason = FText::FromString(TEXT("Unknown Error")))
	{
		Code = SSOperationCode::CD_ERROR;
		Message = Reason;
	}

	/**
	 * Converts the templated C++ operation result into a Blueprint-compatible result.
	 * 
	 * Requires that T is a USTRUCT and registered with the reflection system.
	 */
	FSSOperationResult ToBlueprintResult() const
	{
		FSSOperationResult BPResult;
		BPResult.Code = Code;
		BPResult.Message = Message;

		static_assert(TSSIsUStruct<T>::Value, "T must be a USTRUCT to convert to FInstancedStruct");

		BPResult.Data = FInstancedStruct::Make(Data);

		return BPResult;
	}
};