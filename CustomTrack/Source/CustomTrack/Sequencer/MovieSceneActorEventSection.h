#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "Curves/CurveInterface.h"

#include "MyActor.h"

#if WITH_EDITOR
#include "MovieSceneClipboard.h"
#endif


#include "MovieSceneActorEventSection.generated.h"

USTRUCT()
struct FActorEventPayload
{
	GENERATED_BODY()

	FActorEventPayload() {}

	FActorEventPayload(const FActorEventPayload&) = default;
	FActorEventPayload& operator=(const FActorEventPayload&) = default;

	FActorEventPayload(FActorEventPayload&&) = default;
	FActorEventPayload& operator=(FActorEventPayload&&) = default;

	UPROPERTY(EditAnywhere, Category = Event)
	bool BoolValue = false;

	UPROPERTY(EditAnywhere, Category = Event)
	FName NameValue;
};

using FActorEventPaylaodCurve = TCurveInterface<FActorEventPayload, float> ;

#if WITH_EDITOR

namespace MovieSceneClipboard
{
	template<> inline FName GetKeyTypeName<FActorEventPayload>()
	{
		return "ActorEventPayload";
	}
}

#endif


USTRUCT()
struct FMovieSceneActorEventSectionData
{
	GENERATED_BODY()

	FMovieSceneActorEventSectionData() = default;

	FMovieSceneActorEventSectionData(const FMovieSceneActorEventSectionData& RHS)
		: KeyTimes(RHS.KeyTimes)
		, KeyValues(RHS.KeyValues)
	{
	}

	FMovieSceneActorEventSectionData& operator=(const FMovieSceneActorEventSectionData& RHS)
	{
		KeyTimes = RHS.KeyTimes;
		KeyValues = RHS.KeyValues;
#if WITH_EDITORONLY_DATA
		KeyHandles.Reset();
#endif
		return *this;
	}

	/** Sorted array of key times */
	UPROPERTY()
	TArray<float> KeyTimes;

	/** Array of values that correspond to each key time */
	UPROPERTY()
	TArray<FActorEventPayload> KeyValues;

#if WITH_EDITORONLY_DATA
	/** Transient key handles */
	FKeyHandleLookupTable KeyHandles;
#endif
};


UCLASS()
class CUSTOMTRACK_API UMovieSceneActorEventSection
	: public UMovieSceneSection
{
	GENERATED_BODY()

	UMovieSceneActorEventSection();

public:

	const FMovieSceneActorEventSectionData& GetEventData() const { return EventData; }

	FActorEventPaylaodCurve GetCurveInterface() { return CurveInterface.GetValue(); }

public:

	//~ UMovieSceneSection interface

	virtual void DilateSection(float DilationFactor, float Origin, TSet<FKeyHandle>& KeyHandles) override;
	virtual void GetKeyHandles(TSet<FKeyHandle>& KeyHandles, TRange<float> TimeRange) const override;
	virtual void MoveSection(float DeltaPosition, TSet<FKeyHandle>& KeyHandles) override;
	virtual TOptional<float> GetKeyTime(FKeyHandle KeyHandle) const override;
	virtual void SetKeyTime(FKeyHandle KeyHandle, float Time) override;

private:

	UPROPERTY()
	FMovieSceneActorEventSectionData EventData;

	TOptional<FActorEventPaylaodCurve> CurveInterface;
};
