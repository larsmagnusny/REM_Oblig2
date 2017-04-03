// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"
#include "ShadowScript.h"

UShadowScript::UShadowScript()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UShadowScript::BeginPlay()
{
	Super::BeginPlay();

	if(Trigger)
		Trigger->OnActorBeginOverlap.AddDynamic(this, &UShadowScript::OnOverlapBegin);

	OriginalPosition = GetOwner()->GetActorLocation();
	CurrentPosition = OriginalPosition;
}

void UShadowScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (Animating)
	{
		UE_LOG(LogTemp, Error, TEXT("Animating"));
		FVector Direction = FinalPosition - CurrentPosition;
		
		float Distance = Direction.Size();

		FVector FinalR = FVector(FinalRotation.Roll, FinalRotation.Yaw, FinalRotation.Pitch);
		FRotator CurrentRotation = GetOwner()->GetActorRotation();
		FVector CurrentR = FVector(CurrentRotation.Roll, CurrentRotation.Yaw, CurrentRotation.Pitch);

		FVector RDirection = FinalR - CurrentR;

		float RDistance = RDirection.Size();

		if (RDistance > 0.5f)
		{
			RDirection.Normalize();
			CurrentRotation.Roll = CurrentRotation.Roll + RDirection.X*Speed*0.5*DeltaTime;
			CurrentRotation.Yaw = CurrentRotation.Yaw + RDirection.Y*Speed*0.5*DeltaTime;
			CurrentRotation.Pitch = CurrentRotation.Pitch + RDirection.Z*Speed*0.5*DeltaTime;

			GetOwner()->SetActorRotation(CurrentRotation);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Done Rolling!"));
			GetOwner()->SetActorRotation(FinalRotation);
		}


		if (Distance > 10.f)
		{
			UE_LOG(LogTemp, Error, TEXT("Moving"));
			Direction.Normalize();
			CurrentPosition += Direction*Speed*DeltaTime;
			GetOwner()->SetActorLocation(CurrentPosition);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Done"));
			Animating = false;
			GetOwner()->SetActorLocation(FinalPosition);
			CanEverTrigger = false;
		}
	}
}

FBufferArchive UShadowScript::GetSaveData()
{
	FBufferArchive BinaryData;

	FVector Location = GetOwner()->GetActorLocation();
	FRotator Rotation = GetOwner()->GetActorRotation();

	BinaryData << Location;
	BinaryData << Rotation;
	BinaryData << CanEverTrigger;
	return BinaryData;
}

void UShadowScript::LoadSaveData(FMemoryReader & Ar)
{
	FVector Location;
	FRotator Rotation;
	Ar << Location;
	Ar << Rotation;
	Ar << CanEverTrigger;

	GetOwner()->SetActorLocation(Location);
	GetOwner()->SetActorRotation(Rotation);
}

void UShadowScript::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	// Sjekk om det er HovedKarakteren vi overlapper
	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		// Cast til Klassen så vi kan bruke dens funksjoner
		AMainCharacter* OurCharacter = Cast<AMainCharacter>(OtherActor);

		UE_LOG(LogTemp, Error, TEXT("Character Walked over it..."));

		if (CanEverTrigger)
		{
			Animating = true;
		}
	}
}