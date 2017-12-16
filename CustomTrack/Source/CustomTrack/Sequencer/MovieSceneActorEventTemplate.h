// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MovieSceneActorEventSection.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "MovieSceneObjectBindingID.h"

#include "MovieSceneActorEventTemplate.generated.h"

class UMovieSceneActorEventTrack;
struct EventData;

USTRUCT()
struct FMovieSceneActorEventTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
	
	FMovieSceneActorEventTemplate() {}
	FMovieSceneActorEventTemplate(const UMovieSceneActorEventSection& Section, const UMovieSceneActorEventTrack& Track);

	UPROPERTY()
	FMovieSceneActorEventSectionData EventData;

	UPROPERTY()
	TArray<FMovieSceneObjectBindingID> EventReceivers;

	UPROPERTY()
	uint32 bFireEventsWhenForwards : 1;

	UPROPERTY()
	uint32 bFireEventsWhenBackwards : 1;

private:

	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
};
