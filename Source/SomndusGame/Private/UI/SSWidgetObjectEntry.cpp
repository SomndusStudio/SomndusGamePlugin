/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSWidgetObjectEntry.h"

void ISSWidgetObjectEntry::NativeOnItemObjectSet(UObject* InItemObject)
{
	Execute_OnItemObjectSet(Cast<UObject>(this), InItemObject);
}