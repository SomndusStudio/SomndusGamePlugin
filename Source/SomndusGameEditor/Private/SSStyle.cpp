/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "SSStyle.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FSSStyle::StyleInstance = NULL;

void FSSStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSSStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSSStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SSStyle"));
	return StyleSetName;
}

TSharedPtr<ISlateStyle> FSSStyle::Get()
{
	return StyleInstance;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

TSharedRef<FSlateStyleSet> FSSStyle::Create()
{
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon128x128(128.0f, 128.0f);

	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("SSStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("SomndusGame")->GetBaseDir() / TEXT("Resources"));

	// Asset icon/thumbnail
	Style->Set("NarrativeGraph.PinIcon.Small", new FSlateImageBrush(
		           Style->RootToContentDir(TEXT("NarrativePinCircle_Icon32.png")), FVector2D(32.f, 32.f))
	);

	Style->Set("NarrativeGraph.PinIcon.Tiny", new FSlateImageBrush(
				   Style->RootToContentDir(TEXT("NarrativePinCircle_Icon16.png")), Icon16x16)
	);
	
	Style->Set("NarrativeGraph.PinHover.Tiny", new FSlateImageBrush(
				   Style->RootToContentDir(TEXT("NarrativePinCircle_Hover_Icon16.png")), Icon16x16)
	);
	
	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
