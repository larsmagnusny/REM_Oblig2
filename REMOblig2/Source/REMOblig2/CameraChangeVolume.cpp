// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "CameraChangeVolume.h"

ACameraChangeVolume::ACameraChangeVolume()
{
	
}

void ACameraChangeVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACameraChangeVolume::OnOverlapBegin);
}

void ACameraChangeVolume::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Should change cameraview to specified angle now!"));
	
	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		AMainCharacter* OurCharacter = Cast<AMainCharacter>(OtherActor);

		OurCharacter->ChangeCameraView(CameraAngle);
	}
}