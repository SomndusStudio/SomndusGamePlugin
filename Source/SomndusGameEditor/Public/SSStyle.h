/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "SlateBasics.h"
#include "ClassIconFinder.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

/**
 * 
 */
class SOMNDUSGAMEEDITOR_API FSSStyle{
public:
	static void Initialize();
	static void Shutdown();
	static FName GetStyleSetName();
	static TSharedPtr<class ISlateStyle> Get();

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;

};