/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Player/SSLocalPlayer.h"

#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

UGameUserSettings* USSLocalPlayer::GetGameUserSettings() const
{
	return GEngine ? GEngine->GetGameUserSettings() : nullptr;
}
