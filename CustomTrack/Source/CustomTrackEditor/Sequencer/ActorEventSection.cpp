
#include "ActorEventSection.h"

#include "ISectionLayoutBuilder.h"
#include "SequencerSectionPainter.h"
#include "Sequencer/MovieSceneActorEventSection.h"
#include "GenericKeyArea.h"

#define LOCTEXT_NAMESPACE "FActorEventSection"

FActorEventSection::FActorEventSection(UMovieSceneSection& InSection, TSharedPtr<ISequencer> InSequencer)
	: Section(Cast<UMovieSceneActorEventSection>(&InSection))
	, Sequencer(InSequencer)
{ }

UMovieSceneSection* FActorEventSection::GetSectionObject()
{
	return Section;
}

int32 FActorEventSection::OnPaintSection( FSequencerSectionPainter& InPainter ) const
{
	return InPainter.PaintSectionBackground();
}

void FActorEventSection::GenerateSectionLayout(ISectionLayoutBuilder& LayoutBuilder) const
{
	auto KeyArea = MakeShared<TGenericKeyArea<FActorEventPayload, float>>(Section->GetCurveInterface(), Section);
	LayoutBuilder.SetSectionAsKeyArea(KeyArea);
}


#undef LOCTEXT_NAMESPACE
