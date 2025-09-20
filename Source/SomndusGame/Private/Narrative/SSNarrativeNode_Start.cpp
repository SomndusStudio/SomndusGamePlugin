// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeNode_Start.h"

USSNarrativeNode_Start::USSNarrativeNode_Start()
{
	DefaultEditorNodeColor = FLinearColor(0.1f, 0.6f, 1.0f);
}

FText USSNarrativeNode_Start::GetDisplayName_Implementation() const
{
	return FText::FromString("Root");
}
