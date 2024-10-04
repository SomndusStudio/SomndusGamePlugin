/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSDAOObjectBase.h"
#include "Core/SSGameModule.h"
#include "SSDAOManager.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSDAOManager : public USSGameModule
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<USSDAOObjectBase*> DAOObjects;

	UFUNCTION(BlueprintCallable, Category="AFT", meta = (DeterminesOutputType = "DAOClass", CompactNodeTitle="GetDAO"))
	USSDAOObjectBase* GetDAO(TSubclassOf<USSDAOObjectBase> DAOClass);
	
	template<class T>
	T* GetDAO()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const USSDAOObjectBase>::Value, "'T' template parameter to GetDAO must be derived from USSDAOObjectBase");

		return (T*) GetDAO(T::StaticClass());
	}
};
