// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "SSNetContextObject.generated.h"

/**
 * USSNetContextObject
 *
 * A base UObject class supporting custom replication logic, including optional replication control
 * and support for push-based property updates. Designed to be used with replicated subsystems or
 * context-based replicated UObjects.
 */
UCLASS()
class SOMNDUSGAME_API USSNetContextObject : public USSContextObject
{
	GENERATED_BODY()

public:

	/** Whether this object should replicate. Can be changed at runtime. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category=ObjectReplication,meta=(DisplayName = "Object Replicates", AllowPrivateAccess = "true"))
	uint8 bReplicates:1;
	
	bool GetIsReplicated() const
	{
		return bReplicates;
	}

	/**
	 * Sets whether this object should replicate by default.
	 *
	 * This modifies the `bReplicates` flag and marks it dirty so that the replication system
	 * is notified of the change.
	 *
	 * @param bNewReplicates	New replicate state to apply.
	 */
	void SetIsReplicatedByDefault(const bool bNewReplicates);
	
protected:

	virtual bool IsSupportedForNetworking() const override
	{
		return GetIsReplicated();
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override
	{
		if ((Function->FunctionFlags & FUNC_Static))
		{
			// Try to use the same logic as function libraries for static functions, will try to use the global context to check authority only/cosmetic
			return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
		}
		
		check(GetOuter() != nullptr);
		return GetOuter()->GetFunctionCallspace(Function, Stack);
	}

	// Call "Remote" (aka, RPC) functions through the actors NetDriver
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override
	{
		bool Called = false;
		UActorComponent* Outer = Cast<UActorComponent>(GetOuter());
		AActor* Actor = Outer ? Outer->GetOwner() : nullptr;
		UNetDriver* NetDriver = Actor ? Actor->GetNetDriver() : nullptr;
		if (NetDriver)
		{
			NetDriver->ProcessRemoteFunction(Actor, Function, Parms, OutParms, Stack, this);
			Called = true;
		}
		return Called;
	}
};
