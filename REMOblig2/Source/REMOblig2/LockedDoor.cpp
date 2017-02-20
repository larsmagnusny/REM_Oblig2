// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LockedDoor.h"
#include "REM_GameMode.h"

ULockedDoor::ULockedDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockedDoor::BeginPlay()
{
	Super::BeginPlay();
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	GameMode->AddInteractableObject(GetOwner(), this);
	InitialRotation = GetOwner()->GetActorRotation();
}

void ULockedDoor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Open)
	{
		if (CurrentRotation < MaxOpenAngle)
		{
			CurrentRotation += OpenSpeed*DeltaTime;
		}
		else
		{
			CurrentRotation = MaxOpenAngle;
		}

		FRotator NewRotation = InitialRotation;

		if (OpenDirection)
			NewRotation.Yaw += CurrentRotation;
		else
			NewRotation.Yaw -= CurrentRotation;

		GetOwner()->SetActorRotation(NewRotation.Quaternion(), ETeleportType::None);
	}
	else {
		if (CurrentRotation > 0)
		{
			CurrentRotation -= OpenSpeed*DeltaTime;
		}
		else {
			CurrentRotation = 0.f;
		}

		FRotator NewRotation = InitialRotation;

		if (OpenDirection)
			NewRotation.Yaw += CurrentRotation;
		else
			NewRotation.Yaw -= CurrentRotation;

		GetOwner()->SetActorRotation(NewRotation.Quaternion(), ETeleportType::None);
	}
}

void ULockedDoor::ActivateObject()
{
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		if (Open)
			CloseDoor();
		else
			OpenDoor();
	}
}

void ULockedDoor::OpenDoor()
{
	Open = true;
}

void ULockedDoor::CloseDoor()
{
	Open = false;
}

void ULockedDoor::UnlockDoor(InventoryItem* item)
{

}

void ULockedDoor::LockDoor(InventoryItem* item)
{

}

void ULockedDoor::SetPuzzleSolved()
{
	PuzzleSolved = true;
}