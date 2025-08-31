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
 * @class USSGameplayHelperStatics
 * A utility class providing static functions to manage and play Level Sequences in Unreal Engine.
 *
 * This class extends UBlueprintFunctionLibrary to expose functionality for creating and managing
 * Level Sequence Players at specified transforms or actors. These functionalities can be used
 * in both C++ and Blueprint to handle sequences dynamically in the game.
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
