// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeContext.h"

FSSNarrativeProfile* USSNarrativeContext::GetProfile(FName ProfileId)
{
	for (auto& NarrativeProfile : NarrativeProfiles)
	{
		if (NarrativeProfile.ProfileId == ProfileId) return &NarrativeProfile;
	}

	return &NarrativeProfileFallback;
}

void USSNarrativeContext::LoadFromSaveData(const FSSNarrativeSaveData* SaveData)
{
	ActiveNarrativeTags = SaveData->ActiveNarrativeTags;
	Flags = SaveData->GlobalFlags;
	Tags =  SaveData->GlobalTags;
	NarrativeProfiles = SaveData->SavedProfiles;
}

FSSNarrativeSaveData USSNarrativeContext::GetSave() const
{
	FSSNarrativeSaveData SaveData = FSSNarrativeSaveData();
	SaveData.ActiveNarrativeTags = ActiveNarrativeTags;;
	SaveData.GlobalFlags = Flags;
	SaveData.GlobalTags = Tags;
	SaveData.SavedProfiles = NarrativeProfiles;

	return SaveData;
}
