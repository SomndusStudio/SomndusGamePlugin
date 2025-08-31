/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "SSContextObject.h"

#include "Kismet/GameplayStatics.h"

void USSContextObject::PostLoad()
{
	UObject::PostLoad();
}

ULevel* USSContextObject::GetLevel() const
{
	return GetTypedOuter<ULevel>();
}

UGameInstance* USSContextObject::GetGameInstance() const
{
	return UGameplayStatics::GetGameInstance(this);
}

class UWorld* USSContextObject::GetWorld() const {
	// If the actors outer is destroyed or unreachable we are shutting down and the world should be nullptr
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("Actor: %s has a null OuterPrivate in UDUObject::GetWorld()"), *GetFullName())
		&& !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}