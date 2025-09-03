// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SSGameplayStatics.h"

#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

bool USSGameplayStatics::Actor_IsPrimaryPlayer(AActor* Actor)
{
	auto* ActorPawn = Cast<APawn>(Actor);
	if (!ActorPawn) return false;
	
	auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(Actor, 0);
	if (!PlayerPawn) return false;
	
	return ActorPawn == PlayerPawn;
}
