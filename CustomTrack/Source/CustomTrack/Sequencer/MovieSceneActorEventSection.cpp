
#include "MovieSceneActorEventSection.h"

#include "EngineGlobals.h"
#include "LinkerLoad.h"
#include "Curves/KeyFrameAlgorithms.h"




//// UMovieSceneActorEventSection

UMovieSceneActorEventSection::UMovieSceneActorEventSection()
#if WITH_EDITORONLY_DATA
	: CurveInterface(FActorEventPaylaodCurve(&EventData.KeyTimes, &EventData.KeyValues, &EventData.KeyHandles))
#else
	: CurveInterface(FActorEventPaylaodCurve(&EventData.KeyTimes, &EventData.KeyValues))
#endif
{
	SetIsInfinite(true);
}


/* UMovieSceneSection overrides
*****************************************************************************/

void UMovieSceneActorEventSection::DilateSection(float DilationFactor, float Origin, TSet<FKeyHandle>& KeyHandles)
{
	Super::DilateSection(DilationFactor, Origin, KeyHandles);

	KeyFrameAlgorithms::Scale(CurveInterface.GetValue(), Origin, DilationFactor, KeyHandles);
}


void UMovieSceneActorEventSection::GetKeyHandles(TSet<FKeyHandle>& KeyHandles, TRange<float> TimeRange) const
{
	for (auto It(CurveInterface->IterateKeys()); It; ++It)
	{
		if (TimeRange.Contains(*It))
		{
			KeyHandles.Add(It.GetKeyHandle());
		}
	}
}


void UMovieSceneActorEventSection::MoveSection(float DeltaPosition, TSet<FKeyHandle>& KeyHandles)
{
	Super::MoveSection(DeltaPosition, KeyHandles);

	KeyFrameAlgorithms::Translate(CurveInterface.GetValue(), DeltaPosition, KeyHandles);
}


TOptional<float> UMovieSceneActorEventSection::GetKeyTime(FKeyHandle KeyHandle) const
{
	return CurveInterface->GetKeyTime(KeyHandle);
}


void UMovieSceneActorEventSection::SetKeyTime(FKeyHandle KeyHandle, float Time)
{
	CurveInterface->SetKeyTime(KeyHandle, Time);
}
