/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSGameModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<USSGameModule*> GameModules;

	UFUNCTION(BlueprintCallable, Category="AFT", meta = (DeterminesOutputType = "GameModuleClass", CompactNodeTitle="GetGameModule"))
	USSGameModule* GetGameModule(TSubclassOf<USSGameModule> GameModuleClass);
	
	template<class T>
	T* GetGameModule()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const USSGameModule>::Value, "'T' template parameter to GetGameModule must be derived from USSGameModule");

		return (T*) GetGameModule(T::StaticClass());
	}
};