// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "RiddlePuzzle.h"
#include "MainCharacter.h"


// Sets default values for this component's properties
URiddlePuzzle::URiddlePuzzle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URiddlePuzzle::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URiddlePuzzle::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

