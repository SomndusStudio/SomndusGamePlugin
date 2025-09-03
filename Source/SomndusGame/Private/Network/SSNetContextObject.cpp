// © 2024 Schartier Isaac. All rights reserved.


#include "Network/SSNetContextObject.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void USSNetContextObject::SetIsReplicatedByDefault(const bool bNewReplicates)
{
	bReplicates = bNewReplicates;
	MARK_PROPERTY_DIRTY_FROM_NAME(USSNetContextObject, bReplicates, this);
}

void USSNetContextObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// allow BP Replication
	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass != NULL)
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
	
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(USSNetContextObject, bReplicates, SharedParams);
}
