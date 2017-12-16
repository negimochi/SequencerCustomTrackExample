// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTrackEditor.h"
#include "Modules/ModuleManager.h"

#include "ISequencerModule.h"

#include "Sequencer/ActorEventTrackEditor.h"

void FCustomTrackEditor::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	ActorEventTrackEditorHandle = SequencerModule.RegisterTrackEditor(
		FOnCreateTrackEditor::CreateStatic(&FActorEventTrackEditor::CreateTrackEditor));

}

void FCustomTrackEditor::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
	{
		return;
	}

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(ActorEventTrackEditorHandle);
}

IMPLEMENT_PRIMARY_GAME_MODULE(FCustomTrackEditor, CustomTrackEditor, "CustomTrackEditor" );

