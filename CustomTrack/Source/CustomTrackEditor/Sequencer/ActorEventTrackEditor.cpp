#include "ActorEventTrackEditor.h"

#include "Misc/Guid.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EditorStyleSet.h"
#include "SequencerUtilities.h"

#include "Sequencer/MovieSceneActorEventTrack.h"
#include "ActorEventSection.h"

#include "MovieScene.h"

#include "MyActor.h"


#define LOCTEXT_NAMESPACE "FActorEventTrackEditor"

FActorEventTrackEditor::FActorEventTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{ }


TSharedRef<ISequencerTrackEditor> FActorEventTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FActorEventTrackEditor(InSequencer));
}

bool FActorEventTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	// レベルシーケンスのみ許可
	// 他のコードをみても、こんな感じで GetName で判定している
	return (InSequence != nullptr) && (InSequence->GetClass()->GetName() == TEXT("LevelSequence"));
}

bool FActorEventTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UMovieSceneActorEventTrack::StaticClass();
}

TSharedRef<ISequencerSection> FActorEventTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FActorEventSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FActorEventTrackEditor::GetIconBrush() const
{
	// イベントトラックと同じアイコン
	return FEditorStyle::GetBrush("Sequencer.Tracks.Event");
}

void FActorEventTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding, const UClass* ObjectClass)
{
	// 念のためアクター限定としておく
	if (ObjectClass->IsChildOf(AMyActor::StaticClass()))
	{
		MenuBuilder.AddMenuEntry(
			NSLOCTEXT("Sequencer", "AddActorEvent", "Actor Event"),
			NSLOCTEXT("Sequencer", "AddActorEventTooltip", "MyActor用 Event Track を追加する"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FActorEventTrackEditor::FindOrCreateTrack, ObjectBinding)
			)
		);
	}
}

void FActorEventTrackEditor::FindOrCreateTrack(FGuid ObjectHandle)
{
	// トラックがなかったら作成する
	FFindOrCreateTrackResult TrackResult = FindOrCreateTrackForObject(ObjectHandle, UMovieSceneActorEventTrack::StaticClass(), TEXT("Actor Event"));
	auto* NewTrack = CastChecked<UMovieSceneActorEventTrack>(TrackResult.Track, ECastCheckedType::NullAllowed);

	if (TrackResult.bWasCreated)
	{
		// トラック作成成功

		// トラック側に BindingID をセット
		FMovieSceneObjectBindingID BindingID(ObjectHandle, GetSequencer()->GetFocusedTemplateID());
		NewTrack->EventReceivers.Add(BindingID);

		// セクション追加
		UMovieSceneSection* NewSection = NewTrack->CreateNewSection();
		check(NewSection);
		NewTrack->AddSection(*NewSection);
		NewTrack->SetDisplayName(LOCTEXT("TrackName", "Actor Events"));

	}

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}


#undef LOCTEXT_NAMESPACE
