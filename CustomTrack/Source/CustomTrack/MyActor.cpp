// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyActor::CallCppFunction(float Time, bool TestBool, FName TestName)
{
	auto LogStr = FString::Printf(TEXT("[time:%f] CallCppFunction: %s, %s"), Time, (TestBool) ? TEXT("True") : TEXT("False"), *TestName.ToString());
	UKismetSystemLibrary::PrintString(GetWorld(), LogStr);
}
