/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Setting/SSSettingDataHandler.h"

void USSSettingDataHandler::NativeApply(TObjectPtr<USSSettingDataObject> InData)
{
	OnApply(InData);
}

UWorld* USSSettingDataHandler::GetWorld() const
{
	if (!WorldObjectContext)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : For some reason the handler don't have a valid world object context"),  *GetNameSafe(this));
		
		return nullptr;
	}
	return WorldObjectContext->GetWorld();
}
