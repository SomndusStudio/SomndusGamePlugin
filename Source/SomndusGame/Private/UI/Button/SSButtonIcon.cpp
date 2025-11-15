// ©2025 Schartier Isaac. All rights reserved.


#include "UI/Button/SSButtonIcon.h"

#include "Components/Image.h"


void USSButtonIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (bShowIcon)
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USSButtonIcon::SetIcon(UTexture2D* InIcon)
{
	if (IconImage && bShowIcon)
	{
		IconImage->SetBrushFromTexture(InIcon);
		IconImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USSButtonIcon::SetShowIcon(bool InShowIcon)
{
	bShowIcon = InShowIcon;

	if (bShowIcon)
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
