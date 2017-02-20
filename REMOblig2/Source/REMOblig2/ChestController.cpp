// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "ChestController.h"


// Sets default values for this component's properties
UChestController::UChestController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChestController::BeginPlay()
{
	Super::BeginPlay();

	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}


// Called every frame
void UChestController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UChestController::ActivateObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Should open chest now!"));

	isOpen = !isOpen;
}