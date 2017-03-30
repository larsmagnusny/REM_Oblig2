// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LevelChangeActor.h"


// Sets default values for this component's properties
ULevelChangeActor::ULevelChangeActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelChangeActor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Så GameMode er klar over at vi kan interactes med
	GameMode->AddInteractableObject(GetOwner(), this);
}


// Called every frame
void ULevelChangeActor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void ULevelChangeActor::ActivateObject(AActor* Player)
{
	FLatentActionInfo info;
	UE_LOG(LogTemp, Warning, TEXT("LoadLevel Called!"));
	GameMode->SpawnMap(LevelName);
	GameMode->UnloadMap(GameMode->CurrentLoadedMap);

	ACharacter* OurCharacter = GameMode->GetMainCharacter();
}