/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSGameModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSGameSubsystem.generated.h"

/**
 * USSGameSubsystem
 *
 * A game instance subsystem responsible for managing and providing access to modular game logic units,
 * called "game modules" (`USSGameModule`). Modules are lazily created on demand, stored, and reused.
 *
 * This enables clean separation of systems (combat, dialogue, save/load, etc.) into isolated, reusable modules.
 * Each module can define its own logic and is initialized through `NativeInit()`.
 *
 * Use `GetGameModule<T>()` to retrieve a module in C++, or `GetGameModule(Class)` in Blueprints.
 */
UCLASS()
class SOMNDUSGAME_API USSGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/** State/Bundle to always load on clients */
	static const FName LoadStateGameCore;

	
	/** Internal list of game module instances, stored and reused after creation. */
	UPROPERTY(BlueprintReadOnly)
	TArray<USSGameModule*> GameModules;

	/**
	 * Retrieves an instance of the specified game module class.
	 *
	 * If an instance of the requested module already exists, it is returned.
	 * Otherwise, the module is created, initialized via `NativeInit()`, stored, and returned.
	 *
	 * @param GameModuleClass	The class of the game module to retrieve or create.
	 * @return					A valid instance of the requested module class.
	 */
	UFUNCTION(BlueprintCallable, Category="AFT", meta = (DeterminesOutputType = "GameModuleClass", CompactNodeTitle="GetGameModule"))
	USSGameModule* GetGameModule(TSubclassOf<USSGameModule> GameModuleClass);
	
	/**
	 * Templated version of GetGameModule.
	 *
	 * Retrieves or creates a game module of the specified type.
	 * Ensures the module inherits from USSGameModule at compile time.
	 *
	 * @return A valid pointer to the requested module type.
	 */
	template<class T>
	T* GetGameModule()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const USSGameModule>::Value, "'T' template parameter to GetGameModule must be derived from USSGameModule");

		return static_cast<T*>(GetGameModule(T::StaticClass()));
	}
};