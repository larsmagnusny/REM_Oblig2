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
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	GameMode->AddInteractableObject(GetOwner(), this);
}

void ULockedDoor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void ULockedDoor::ActivateObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Door has been activated!"));
}

void ULockedDoor::OpenDoor()
{

}

void ULockedDoor::CloseDoor()
{

}

void ULockedDoor::UnlockDoor(InventoryItem* item)
{

}

void ULockedDoor::LockDoor(InventoryItem* item)
{

}