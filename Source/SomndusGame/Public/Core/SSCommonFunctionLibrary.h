// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSGameModule.h"
#include "DAO/SSDAOObjectBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSCommonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Somndus Studio", meta = (WorldContext = "ContextObject", BlueprintInternalUseOnly = "true"))
	static USSGameModule* GetGameModule(UObject* ContextObject, TSubclassOf<USSGameModule> Class);

	template<class T>
	static T* GetGameModuleByClass(UObject* ContextObject)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const USSGameModule>::Value, "'T' template parameter to GetGameModule must be derived from USSGameModule");

		return (T*) GetGameModule(ContextObject, T::StaticClass());
	}
	
	UFUNCTION(BlueprintPure, Category = "Somndus Studio", meta = (WorldContext = "ContextObject", BlueprintInternalUseOnly = "true"))
	static USSDAOObjectBase* GetDAOByClass(UObject* ContextObject, TSubclassOf<USSDAOObjectBase> Class);
};
