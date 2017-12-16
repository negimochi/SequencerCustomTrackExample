// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"



UCLASS()
class CUSTOMTRACK_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AMyActor();

	UFUNCTION()
	void CallCppFunction(float Time, bool TestBool, FName TestName);
};
