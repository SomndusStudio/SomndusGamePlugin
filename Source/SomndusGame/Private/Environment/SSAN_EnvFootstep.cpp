/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Environment/SSAN_EnvFootstep.h"

#include "Components/SkeletalMeshComponent.h"
#include "Environment/SSEnvironmentComponent.h"

void USSAN_EnvFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	
	auto* EnvironmentComponent = MeshComp->GetOwner()->FindComponentByClass<USSEnvironmentComponent>();
	if (!EnvironmentComponent) return;

	EnvironmentComponent->ExecuteFootstep(FootstepTag);
}
