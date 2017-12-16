// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CustomTrackEditor : ModuleRules
{
	public CustomTrackEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd", "CustomTrack"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate", "SlateCore", "EditorStyle",
            "Sequencer", "MovieScene", "MovieSceneTools", "MovieSceneTracks"
        });
	}
}
