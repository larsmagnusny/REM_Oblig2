// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"

#include "ChestSpot.h"

UChestSpot::UChestSpot()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UChestSpot::BeginPlay()
{
	Super::BeginPlay();


	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->AddInteractableObject(GetOwner(), this);
}

void UChestSpot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UChestSpot::ActivateObject(AActor* Player)
{
	print("There's an x marking the spot. I wonder if i place something here something will happen?");
}

FVector UChestSpot::GetActivatePosition(AActor* Player)
{
	return GetOwner()->GetActorLocation();
}