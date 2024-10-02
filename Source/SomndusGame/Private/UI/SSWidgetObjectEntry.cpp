// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SSWidgetObjectEntry.h"

void ISSWidgetObjectEntry::NativeOnItemObjectSet(UObject* InItemObject)
{
	Execute_OnItemObjectSet(Cast<UObject>(this), InItemObject);
}