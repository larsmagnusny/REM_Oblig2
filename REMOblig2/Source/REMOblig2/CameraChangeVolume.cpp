// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "CameraChangeVolume.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"

ACameraChangeVolume::ACameraChangeVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ACameraChangeVolume::BeginPlay()
{
	Super::BeginPlay();

	// Fortell Unreal at den skal legge til en overlap-event for dette objektet
	OnActorBeginOverlap.AddDynamic(this, &ACameraChangeVolume::OnOverlapBegin);
}

void ACameraChangeVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SetMousePositionNextFrame)
	{
		if (FrameCounter < FramesToWait)
		{
			FrameCounter++;
		}
		else {
			FrameCounter = 0;
			SetMousePositionNextFrame = false;

			// After camera has been changed...
			FVector2D NewMousePosition;

			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(WorldPosition, NewMousePosition, false);

			SetMousePosition(GetWorld()->GetFirstPlayerController(), NewMousePosition.X, NewMousePosition.Y);
		}
	}
}



void ACameraChangeVolume::SetMousePosition(APlayerController* Controller, float X, float Y)
{
	if (Controller)
	{
		ULocalPlayer* Player = Cast<ULocalPlayer>(Controller->Player);

		if (Player)
		{
			FViewport* v = Player->ViewportClient->Viewport;

			if(v)
				v->SetMouse((int)X, (int)Y);
		}
	}
}

void ACameraChangeVolume::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (IgnoreNextOverlap)
	{
		IgnoreNextOverlap = false;
		return;
	}

	// Sjekk om det er HovedKarakteren vi overlapper
	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		// Cast til Klassen s� vi kan bruke dens funksjoner
		AMainCharacter* OurCharacter = Cast<AMainCharacter>(OtherActor);

		// Endre p� kameravinkelen til den gitte Vektoren
		if (CameraToSwitchTo)
		{
			// Hent en posisjon litt foran kameraet...

			if (IgnoreThisWhenOverlap)
			{
				ACameraChangeVolume* IgnoreThis = Cast<ACameraChangeVolume>(IgnoreThisWhenOverlap);

				IgnoreThis->IgnoreNextOverlap = true;
			}

			APlayerController* Controller = GetWorld()->GetFirstPlayerController();

			FHitResult Hit;

			Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, Hit);
			
			WorldPosition = Hit.ImpactPoint;

			APlayerCameraManager* CameraManager = Controller->PlayerCameraManager;

			AActor* Target = CameraManager->GetViewTarget();


			if(OurCharacter && Target != CameraToSwitchTo)
			{
				OurCharacter->ChangeCameraView(CameraToSwitchTo);

				SetMousePositionNextFrame = true;
			}
		}
	}
}