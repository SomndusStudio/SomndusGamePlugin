// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSGameplayHelperStatics.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameplayHelperStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "LevelSequence", meta=(WorldContext="WorldContextObject"))
	static ULevelSequencePlayer* CreateLevelSequencePlayerAtTransform(UObject* WorldContextObject, ULevelSequence* InLevelSequence, FMovieSceneSequencePlaybackSettings Settings, const FTransform& Transform, ALevelSequenceActor*& OutActor);

	UFUNCTION(BlueprintCallable, Category = "LevelSequence", meta=(WorldContext="WorldContextObject"))
	static ULevelSequencePlayer* CreateLevelSequencePlayerAtActor(UObject* WorldContextObject, ULevelSequence* InLevelSequence, FMovieSceneSequencePlaybackSettings Settings, AActor* Actor, ALevelSequenceActor*& OutActor);
	
};
