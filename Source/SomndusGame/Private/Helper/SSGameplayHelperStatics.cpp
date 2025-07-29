// © 2024 Schartier Isaac. All rights reserved.


#include "Helper/SSGameplayHelperStatics.h"

#include "DefaultLevelSequenceInstanceData.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"

ULevelSequencePlayer* USSGameplayHelperStatics::CreateLevelSequencePlayerAtTransform(UObject* WorldContextObject, ULevelSequence* InLevelSequence,
                                                                                     FMovieSceneSequencePlaybackSettings Settings, const FTransform& Transform, ALevelSequenceActor*& OutActor)
{
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(WorldContextObject, InLevelSequence, Settings, OutActor);
	OutActor->bOverrideInstanceData = true;

	UDefaultLevelSequenceInstanceData* DefaultInstanceData = Cast<UDefaultLevelSequenceInstanceData>(OutActor->DefaultInstanceData);

	if (DefaultInstanceData)
	{
		// Snap to Location and Rotation of given transform
		FTransform FinalTransform = DefaultInstanceData->TransformOrigin;
		FinalTransform.SetLocation(Transform.GetLocation());
		FinalTransform.SetRotation(Transform.GetRotation());
		DefaultInstanceData->TransformOrigin = FinalTransform;
	}

	return LevelSequencePlayer;
}

ULevelSequencePlayer* USSGameplayHelperStatics::CreateLevelSequencePlayerAtActor(UObject* WorldContextObject, ULevelSequence* InLevelSequence,
	FMovieSceneSequencePlaybackSettings Settings, AActor* Actor, ALevelSequenceActor*& OutActor)
{
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(WorldContextObject, InLevelSequence, Settings, OutActor);
	OutActor->bOverrideInstanceData = true;

	UDefaultLevelSequenceInstanceData* DefaultInstanceData = Cast<UDefaultLevelSequenceInstanceData>(OutActor->DefaultInstanceData);

	if (DefaultInstanceData)
	{
		DefaultInstanceData->TransformOriginActor = Actor;
	}

	return LevelSequencePlayer;
}
