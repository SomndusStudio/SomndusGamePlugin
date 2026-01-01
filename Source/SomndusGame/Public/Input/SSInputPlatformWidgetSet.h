/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SSInputPlatformWidgetSet.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputPlatformWidget : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName GamepadName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UUserWidget> WidgetClass;
};

/**
 * Store some specific widget by input hardware type (Dual Sense, Dual Shock, Xbox, etc)
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSInputPlatformWidgetSet : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DefaultGamepadName;
	
	UPROPERTY(EditAnywhere, Category="Config")
	TArray<FSSInputPlatformWidget> InputPlatformWidgetSettings;

	UFUNCTION(BlueprintCallable)
	FSSInputPlatformWidget GetSettingFromName(FName GamepadName);

	UFUNCTION(BlueprintCallable)
	bool GetValidSettingFromName(FName GamepadName, FSSInputPlatformWidget& InputPlatformWidgetSetting);
};
