/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Settings/Keyboard/SSKeyInputSetting.h"
#include "SSGameInputData.generated.h"

class UInputAction;

/**
 * Used to sync Input Action binding to another
 * Ex: Weapon Ability with Spirit Ability input
 */
USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSSyncInputActionSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> SourceInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> TargetInputAction;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSDefaultInputSetting : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InputId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MappingName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey Key;
};

class UInputMappingContext;

UCLASS(Blueprintable)
class SOMNDUSGAME_API USSInputActionIdRegistry : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<int32, class UInputAction*> InputActionIdMap;

	UFUNCTION(BlueprintCallable)
	UInputAction* GetInputAction(int32 InputId);
};

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSInputMappingGameModeAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameModeTag;
	
	UPROPERTY(EditDefaultsOnly)
	ECommonInputType InputType;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UInputMappingContext>> MappingContexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> HiddenActionNames;

	//~UPrimaryDataAsset interface
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	//~End of UPrimaryDataAsset interface
};


/**
 * Contain all default setting for a localized input (QWERTY/AZERY) 
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSLocalizedInputSettingTemplate : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSDefaultInputSetting> DefaultMoveInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSDefaultInputSetting> DefaultInputSettings;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSMappableSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> OrderKeyNames;
};


/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameInputData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USSLocalizedInputSettingTemplate> DefaultInputSetting_New;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSSyncInputActionSetting> KeyboardSyncInputSettings_New;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<USSInputMappingGameModeAsset>> InputMappingGameModeAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSSMappableSettings MappableSettings;
};
