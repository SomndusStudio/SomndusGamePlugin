// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Context/SSInputAxisMgr.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

void USSInputAxisMgr::UpdateMoveInputFromIA(const FVector2D& InputAxis, APawn* ControlledPawn)
{
	CachedMoveInput = InputAxis;

	if (!ControlledPawn)
		return;

	AController* Controller = ControlledPawn->GetController();
	if (!Controller)
		return;

	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// Directions dans le monde
	const FVector Forward = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	// Calcul du vecteur de mouvement dans le monde
	RelativeMoveDirection = (Forward * InputAxis.Y + Right * InputAxis.X).GetSafeNormal();

	// Calcul de l'angle d'input
	if (!RelativeMoveDirection.IsNearlyZero())
	{
		FVector PawnForward = ControlledPawn->GetActorForwardVector();
		InputAngleDegrees = UKismetMathLibrary::Acos(FVector::DotProduct(PawnForward, RelativeMoveDirection));
		InputAngleDegrees = FMath::RadiansToDegrees(InputAngleDegrees);

		// Correction du signe de l'angle (gauche ou droite)
		FVector Cross = FVector::CrossProduct(PawnForward, RelativeMoveDirection);
		if (Cross.Z < 0)
		{
			InputAngleDegrees *= -1.f;
		}
	}
	else
	{
		InputAngleDegrees = 0.f;
	}
}

bool USSInputAxisMgr::MoveInputIsZero() const
{
	return CachedMoveInput.IsNearlyZero();
}
