
#include "MovieSceneActorEventTemplate.h"

#include "MovieSceneActorEventTrack.h"

#include "MovieSceneSequence.h"
#include "Evaluation/MovieSceneEvaluationTemplateInstance.h"
#include "EngineGlobals.h"
#include "MovieScene.h"
#include "MovieSceneEvaluation.h"
#include "IMovieScenePlayer.h"

#include "MyActor.h"


struct FMovieSceneEventData
{
	FMovieSceneEventData(const FActorEventPayload& InPayload, float InGlobalPosition) : Payload(InPayload), GlobalPosition(InGlobalPosition) {}

	FActorEventPayload Payload;
	float GlobalPosition;
};

/** A movie scene execution token that stores a specific transform, and an operand */
struct FEventTrackExecutionToken
	: IMovieSceneExecutionToken
{
	FEventTrackExecutionToken(TArray<FMovieSceneEventData> InEvents, const TArray<FMovieSceneObjectBindingID>& InEventReceivers) : Events(MoveTemp(InEvents)), EventReceivers(InEventReceivers) {}

	/** Execute this token, operating on all objects referenced by 'Operand' */
	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{	
		TArray<float> PerformanceCaptureEventPositions;

		// Resolve event contexts to trigger the event on
		TArray<UObject*> EventContexts;

		// If we have specified event receivers, use those
		if (EventReceivers.Num())
		{
			EventContexts.Reserve(EventReceivers.Num());
			for (FMovieSceneObjectBindingID ID : EventReceivers)
			{
				// Ensure that this ID is resolvable from the root, based on the current local sequence ID
				ID = ID.ResolveLocalToRoot(Operand.SequenceID, Player.GetEvaluationTemplate().GetHierarchy());

				// Lookup the object(s) specified by ID in the player
				for (TWeakObjectPtr<> WeakEventContext : Player.FindBoundObjects(ID.GetGuid(), ID.GetSequenceID()))
				{
					if (UObject* EventContext = WeakEventContext.Get())
					{
						EventContexts.Add(EventContext);
					}
				}
			}
		}
		else
		{
			// If we haven't specified event receivers, use the default set defined on the player
			EventContexts = Player.GetEventContexts();
		}

		for (UObject* EventContextObject : EventContexts)
		{
			auto MyActor = Cast<AMyActor>(EventContextObject);
			if (!MyActor)
			{
				continue;
			}

			for (FMovieSceneEventData& Event : Events)
			{
				MyActor->CallCppFunction(Event.GlobalPosition, Event.Payload.BoolValue, Event.Payload.NameValue);
			}
		}
	}


	TArray<FMovieSceneEventData> Events;
	TArray<FMovieSceneObjectBindingID, TInlineAllocator<2>> EventReceivers;
};

FMovieSceneActorEventTemplate::FMovieSceneActorEventTemplate(const UMovieSceneActorEventSection& Section, const UMovieSceneActorEventTrack& Track)
	: EventData(Section.GetEventData())
	, EventReceivers(Track.EventReceivers)
	, bFireEventsWhenForwards(Track.bFireEventsWhenForwards)
	, bFireEventsWhenBackwards(Track.bFireEventsWhenBackwards)
{
}

void FMovieSceneActorEventTemplate::EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Don't allow events to fire when playback is in a stopped state. This can occur when stopping 
	// playback and returning the current position to the start of playback. It's not desireable to have 
	// all the events from the last playback position to the start of playback be fired.
	if (Context.GetStatus() == EMovieScenePlayerStatus::Stopped || Context.IsSilent())
	{
		return;
	}

	const bool bBackwards = Context.GetDirection() == EPlayDirection::Backwards;

	if ((!bBackwards && !bFireEventsWhenForwards) ||
		(bBackwards && !bFireEventsWhenBackwards))
	{
		return;
	}

	TArray<FMovieSceneEventData> Events;

	TRange<float> SweptRange = Context.GetRange();

	const TArray<float>& KeyTimes = EventData.KeyTimes;
	const TArray<FActorEventPayload>& KeyValues = EventData.KeyValues;

	const int32 First = bBackwards ? KeyTimes.Num() - 1 : 0;
	const int32 Last = bBackwards ? 0 : KeyTimes.Num() - 1;
	const int32 Inc = bBackwards ? -1 : 1;

	const float Position = Context.GetTime() * Context.GetRootToSequenceTransform().Inverse();

	if (bBackwards)
	{
		// Trigger events backwards
		for (int32 KeyIndex = KeyTimes.Num() - 1; KeyIndex >= 0; --KeyIndex)
		{
			float Time = KeyTimes[KeyIndex];
			if (SweptRange.Contains(Time))
			{
				Events.Add(FMovieSceneEventData(KeyValues[KeyIndex], Position));
			}
		}
	}
	// Trigger events forwards
	else for (int32 KeyIndex = 0; KeyIndex < KeyTimes.Num(); ++KeyIndex)
	{
		float Time = KeyTimes[KeyIndex];
		if (SweptRange.Contains(Time))
		{
			Events.Add(FMovieSceneEventData(KeyValues[KeyIndex], Position));
		}
	}


	if (Events.Num())
	{
		ExecutionTokens.Add(FEventTrackExecutionToken(MoveTemp(Events), EventReceivers));
	}
}
