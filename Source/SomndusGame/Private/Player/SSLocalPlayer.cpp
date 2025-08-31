// ©2025 Schartier Isaac. All rights reserved.


#include "Player/SSLocalPlayer.h"

#include "GameFramework/GameUserSettings.h"

UGameUserSettings* USSLocalPlayer::GetGameUserSettings() const
{
	return GEngine ? GEngine->GetGameUserSettings() : nullptr;
}
