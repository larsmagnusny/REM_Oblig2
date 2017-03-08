// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LookAtCamera.h"

ALookAtCamera::ALookAtCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALookAtCamera::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
}

void ALookAtCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!LookAt)
	{
		LookAt = GameMode->GetMainCharacter();
		return;
	}

	FVector LookAtPosition = LookAt->GetActorLocation();
	FVector OurPosition = GetActorLocation();

	FVector Dir = LookAtPosition - OurPosition;

	SetActorRotation(Dir.ToOrientationQuat());
}