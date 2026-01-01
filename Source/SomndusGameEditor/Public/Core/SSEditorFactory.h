/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSEditorFactory.generated.h"

////////////////////////////////////////////////////////////////////
// Asset factories

UCLASS()
class USSNarrativeGraph_Factory : public UFactory
{
	GENERATED_BODY()
    
public:
    
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override { return true; }

public:
	USSNarrativeGraph_Factory(const FObjectInitializer& ObjectInitializer);
};