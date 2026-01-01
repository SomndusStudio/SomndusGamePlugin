/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Core/SSEditorFactory.h"

#include "Narrative/SSNarrativeGraph.h"

////////////////////////////////////////////////////////////////////
// Asset factories

USSNarrativeGraph_Factory::USSNarrativeGraph_Factory(const class FObjectInitializer& OBJ)
	: Super(OBJ)
{
	SupportedClass = USSNarrativeGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USSNarrativeGraph_Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
													  EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(USSNarrativeGraph::StaticClass()));
	USSNarrativeGraph* NewGraph = NewObject<USSNarrativeGraph>(InParent, Class, Name, Flags | RF_Transactional, Context);
	
	return NewGraph;
}