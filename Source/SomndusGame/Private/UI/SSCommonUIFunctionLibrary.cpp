/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonUIFunctionLibrary.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "UI/SSWidgetObjectEntry.h"

void USSCommonUIFunctionLibrary::SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject)
{
	if (ISSWidgetObjectEntry* NativeImplementation = Cast<ISSWidgetObjectEntry>(EntryWidget))
	{
		NativeImplementation->NativeOnItemObjectSet(InItemObject);
	}
	else if (EntryWidget->Implements<USSWidgetObjectEntry>())
	{
		ISSWidgetObjectEntry::Execute_OnItemObjectSet(EntryWidget, InItemObject);
	}
}

ULocalPlayer* USSCommonUIFunctionLibrary::GetLocalPlayerFromContext(UObject* WorldContextObject)
{
	ULocalPlayer* LocalPlayer = nullptr;
	if (WorldContextObject)
	{
		if (UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
		{
			LocalPlayer = UserWidget->GetOwningLocalPlayer<ULocalPlayer>();
		}
		else if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
		{
			LocalPlayer = PC->GetLocalPlayer();
		}
		else if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>())
			{
				LocalPlayer = GameInstance->GetPrimaryPlayerController(false)->GetLocalPlayer();
			}
		}
	}
	return LocalPlayer;
}
