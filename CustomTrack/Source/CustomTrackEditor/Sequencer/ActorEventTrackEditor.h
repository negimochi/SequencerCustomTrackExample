#pragma once

#include "CoreMinimal.h"

//#include "Misc/Guid.h"
#include "Templates/SubclassOf.h"
#include "Widgets/SWidget.h"
#include "ISequencer.h"
#include "MovieSceneTrack.h"
#include "ISequencerSection.h"
#include "ISequencerTrackEditor.h"
#include "MovieSceneTrackEditor.h"



class FMenuBuilder;

class FActorEventTrackEditor : public FMovieSceneTrackEditor
{
public:

	FActorEventTrackEditor(TSharedRef<ISequencer> InSequencer);
	virtual ~FActorEventTrackEditor() { }

	// 自分自身のインスタンスを SharedRef で返す関数。
	// モジュール開始時に TrackEditor を生成する関数を登録する必要があるため、多くの場合、ここに static で定義している
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:

	// ISequencerTrackEditor interface

	// セクションの生成
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	// サポートする Sequencer だったときのみ true を返す
	virtual bool SupportsSequence(UMovieSceneSequence* InSequence) const override;
	// サポートするトラックだったときのみ true を返す
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	// アイコンを設定する場合に使う
	virtual const FSlateBrush* GetIconBrush() const override;

	// トラックメニューを構成(+Track ボタンのメニューに追加される)
	//virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	// ObjectBinding 用のトラックメニューを構成(オブジェクトに紐づくトラックの選択メニューに追加される)
	virtual void BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding, const UClass* ObjectClass) override;
	// トラック名横に Widget を追加する
	//virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;

private:
	void FindOrCreateTrack(FGuid ObjectHandle);


};
