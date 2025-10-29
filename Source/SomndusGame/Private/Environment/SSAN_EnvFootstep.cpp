// ©2025 Schartier Isaac. All rights reserved.


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
