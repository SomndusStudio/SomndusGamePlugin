// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSInputAxisMgr.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSInputAxisMgr : public USSContextObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D CachedMoveInput;

	// Move input relatif à la rotation
	UPROPERTY(BlueprintReadOnly)
	FVector RelativeMoveDirection;
	
	// Angle d'input par rapport à la vue du joueur (0 = devant, 180 = derrière)
	UPROPERTY(BlueprintReadOnly)
	float InputAngleDegrees;
	
	// Fonction à appeler à chaque tick d'input
	void UpdateMoveInputFromIA(const FVector2D& InputAxis, APawn* ControlledPawn);

	bool MoveInputIsZero() const;

};
