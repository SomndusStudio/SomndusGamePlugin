/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSSettingWidgetObject.h"
#include "Engine/DataAsset.h"
#include "SSSettingRegistryDataAsset.generated.h"

/**
 * @class USSSettingRegistryDataAsset
 * A blueprintable data asset class used to store a registry of settings for use in UI widgets.
 *
 * This class manages a collection of setting widget objects. These objects are utilized in conjunction
 * with user interface elements to configure and display settings within the game. The settings are stored
 * in the form of a TArray containing instances of USSSettingWidgetObject. Each object contains relevant
 * metadata and functionality related to an individual setting.
 *
 * @details
 * - Settings can be defined, edited, and instanced through the editor or codebase.
 * - Each setting typically includes an identifier, type tags, associated data, and a handler for logic execution.
 * - Designed for use in conjunction with UI components like USSSettingWidget_VList to dynamically create and
 *   manage setting widgets based on this registry.
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSSettingRegistryDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<USSSettingWidgetObject>> Settings;
};
