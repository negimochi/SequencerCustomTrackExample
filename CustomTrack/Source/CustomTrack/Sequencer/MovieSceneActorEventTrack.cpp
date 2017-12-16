// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MovieSceneActorEventTrack.h"

#include "MovieSceneCommonHelpers.h"
#include "MovieSceneActorEventSection.h"
#include "MovieSceneActorEventTemplate.h"

#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "Compilation/IMovieSceneTemplateGenerator.h"
#include "IMovieSceneTracksModule.h"

#define LOCTEXT_NAMESPACE "MovieSceneActorEventTrack"


void UMovieSceneActorEventTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}


UMovieSceneSection* UMovieSceneActorEventTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UMovieSceneActorEventSection::StaticClass(), NAME_None, RF_Transactional);
}

const TArray<UMovieSceneSection*>& UMovieSceneActorEventTrack::GetAllSections() const
{
	return Sections;
}


TRange<float> UMovieSceneActorEventTrack::GetSectionBoundaries() const
{
	TRange<float> SectionBoundaries = TRange<float>::Empty();

	for (auto& Section : Sections)
	{
		SectionBoundaries = TRange<float>::Hull(SectionBoundaries, Section->GetRange());
	}

	return SectionBoundaries;
}


bool UMovieSceneActorEventTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}


bool UMovieSceneActorEventTrack::IsEmpty() const
{
	return (Sections.Num() == 0);
}


void UMovieSceneActorEventTrack::RemoveAllAnimationData()
{
	Sections.Empty();
}


void UMovieSceneActorEventTrack::RemoveSection(UMovieSceneSection& Section)
{
	Sections.Remove(&Section);
}

FMovieSceneEvalTemplatePtr UMovieSceneActorEventTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneActorEventTemplate(*CastChecked<UMovieSceneActorEventSection>(&InSection), *this);
}

void UMovieSceneActorEventTrack::PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const
{
	switch (EventPosition)
	{
	case EFireEventsAtPosition::AtStartOfEvaluation:
		Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::PreEvaluation));
		break;

	case EFireEventsAtPosition::AtEndOfEvaluation:
		Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::PostEvaluation));
		break;

	default:
		Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::SpawnObjects));
		Track.SetEvaluationPriority(UMovieSceneSpawnTrack::GetEvaluationPriority() - 100);
		break;
	}

	Track.SetEvaluationMethod(EEvaluationMethod::Swept);
}

#if WITH_EDITORONLY_DATA

FText UMovieSceneActorEventTrack::GetDefaultDisplayName() const
{ 
	return LOCTEXT("TrackName", "Actor Events"); 
}

#endif


#undef LOCTEXT_NAMESPACE
