// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UnrealEd.h"

class FCustomTrackEditor : public IModuleInterface
{
public:

	void StartupModule() override;
	void ShutdownModule() override;

private:

	FDelegateHandle ActorEventTrackEditorHandle;
};