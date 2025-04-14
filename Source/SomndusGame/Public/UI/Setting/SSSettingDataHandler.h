// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSSettingDataHandler.generated.h"

class USSSettingDataObject;

/**
 * This class is used to determine what behavior to perform when certain actions are applied to the data.
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories)
class SOMNDUSGAME_API USSSettingDataHandler : public USSContextObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldObjectContext;
	
	virtual void NativeApply(TObjectPtr<USSSettingDataObject> InData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnApply(USSSettingDataObject* InData);

	virtual UWorld* GetWorld() const override;
};
