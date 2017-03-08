// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "CameraChangeVolume.h"

ACameraChangeVolume::ACameraChangeVolume()
{
	
}

void ACameraChangeVolume::BeginPlay()
{
	Super::BeginPlay();

	// Fortell Unreal at den skal legge til en overlap-event for dette objektet
	OnActorBeginOverlap.AddDynamic(this, &ACameraChangeVolume::OnOverlapBegin);
}

void ACameraChangeVolume::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	// Sjekk om det er HovedKarakteren vi overlapper
	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera overlapped player"));
		// Cast til Klassen så vi kan bruke dens funksjoner
		AMainCharacter* OurCharacter = Cast<AMainCharacter>(OtherActor);

		// Endre på kameravinkelen til den gitte Vektoren
		if (CameraToSwitchTo)
		{
			OurCharacter->ChangeCameraView(CameraToSwitchTo);
		}
	}
}