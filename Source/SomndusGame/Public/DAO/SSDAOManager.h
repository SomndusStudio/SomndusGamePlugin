/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSDAOObjectBase.h"
#include "Core/SSGameModule.h"
#include "SSDAOManager.generated.h"

/**
 * The USSDAOManager class is responsible for managing DAO (Data Access Object) objects
 * in the game framework. It extends USSGameModule and provides functionality to retrieve
 * or create DAO objects dynamically during runtime.
 */
UCLASS()
class SOMNDUSGAME_API USSDAOManager : public USSGameModule
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<USSDAOObjectBase*> DAOObjects;

	UFUNCTION(BlueprintCallable, Category="Somndus Studio", meta = (DeterminesOutputType = "DAOClass", CompactNodeTitle="GetDAO"))
	USSDAOObjectBase* GetDAO(TSubclassOf<USSDAOObjectBase> DAOClass);
	
	template<class T>
	T* GetDAO()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const USSDAOObjectBase>::Value, "'T' template parameter to GetDAO must be derived from USSDAOObjectBase");

		return static_cast<T*>(GetDAO(T::StaticClass()));
	}
};
