#pragma once

#include "CoreMinimal.h"
#include "ISequencerSection.h"

class FSequencerSectionPainter;
class ISequencer;
class UMovieSceneActorEventSection;


class FActorEventSection
	: public ISequencerSection
{
public:

	FActorEventSection(UMovieSceneSection& InSection, TSharedPtr<ISequencer> InSequencer);

public:

	// ISequencerSection interface

	virtual UMovieSceneSection* GetSectionObject() override;
//	virtual TSharedRef<SWidget> GenerateSectionWidget() override;
	virtual int32 OnPaintSection( FSequencerSectionPainter& InPainter ) const override;
//	virtual void Tick( const FGeometry& AllottedGeometry, const FGeometry& ParentGeometry, const double InCurrentTime, const float InDeltaTime ) override;
//	virtual float GetSectionHeight() const override;
//	virtual float GetSectionGripSize() const override;
	virtual void GenerateSectionLayout( class ISectionLayoutBuilder& LayoutBuilder ) const override;
//	virtual FReply OnSectionDoubleClicked( const FGeometry& SectionGeometry, const FPointerEvent& MouseEvent ) override;
//	virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder) override;

private:

	UMovieSceneActorEventSection* Section;

	TWeakPtr<ISequencer> Sequencer;
};
