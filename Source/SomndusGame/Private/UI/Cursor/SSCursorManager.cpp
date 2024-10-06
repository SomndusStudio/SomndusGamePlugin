/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Cursor/SSCursorManager.h"

#include "Blueprint/UserWidget.h"
#include "UI/SSGameUIManagerSubsystem.h"
#include "UI/SSWidgetObjectEntry.h"
#include "UI/Cursor/SSCursorHandler.h"

void USSCursorManager::Init(const USSGameUIManagerSubsystem* GameUIManagerSubsystem)
{
	auto Class = CursorClass.LoadSynchronous();
	MainCursor = CreateWidget<USSCursorWidgetBase>(GameUIManagerSubsystem->GetGameInstance(), Class);
}

void USSCursorManager::ShowCursor(UUserWidget* Invoker)
{
	if (Invoker->Implements<USSCursorHandler>())
	{
		auto CursorInfo = ISSCursorHandler::Execute_GetCursorRequestInfo(Invoker);
		
		MainCursor->AddToViewport();
		MainCursor->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		MainCursor->SetPositionInViewport(CursorInfo.Position, true);
		MainCursor->NativeUpdateCursorInfo(Invoker);
		MainCursor->UpdateCursorInfo(CursorInfo);
	}
}

void USSCursorManager::HideCursor(UUserWidget* Invoker)
{
	MainCursor->Invoker = nullptr;
	MainCursor->SetVisibility(ESlateVisibility::Collapsed);
	MainCursor->RemoveFromParent();
}
