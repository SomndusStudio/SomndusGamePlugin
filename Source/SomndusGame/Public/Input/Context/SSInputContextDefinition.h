// ©2025 Schartier Isaac. All rights reserved.

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
	//~UPrimaryDataAsset interface
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
	virtual void AddAdditionalAssetBundleData();
#endif
	//~End of UPrimaryDataAsset interface
	
public:
	
	UPROPERTY(EditAnywhere, Category="Input")
	FGameplayTag ContextTag;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftClassPtr<USSInputHandlerContext> InputHandlerContextClass;

	UPROPERTY(EditAnywhere, Category="Input")
	TArray<FSSInputHandlerContextSetting> InputContextSettings;
};