/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SSInputContextDefinition.generated.h"

class USSInputHandlerContext;
class USSInputActionHandler;
class UInputAction;

USTRUCT(BlueprintType)
struct FSSInputHandlerContextSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftClassPtr<USSInputActionHandler> InputActionHandlerClass;
	
};

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputContextDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

	USSInputContextDefinition();

protected:
	
#if WITH_EDITORONLY_DATA
	//~UPrimaryDataAsset interface
	virtual void UpdateAssetBundleData() override;
	virtual void AddAdditionalAssetBundleData();
	//~End of UPrimaryDataAsset interface
#endif
	
public:
	
	UPROPERTY(EditAnywhere, Category="Input")
	FGameplayTag ContextTag;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftClassPtr<USSInputHandlerContext> InputHandlerContextClass;

	UPROPERTY(EditAnywhere, Category="Input")
	TArray<FSSInputHandlerContextSetting> InputContextSettings;
};