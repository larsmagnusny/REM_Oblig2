// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "CustomNavLink.h"

void ACustomNavLink::BeginPlay()
{
	TArray<FNavigationLink> Links = PointLinks;

	//Links[0].Left;
	//Links[0].Right;

	FVector Position = GetActorLocation();
	UBoxComponent* LeftCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision_Left"));
	UBoxComponent* RightCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision_Right"));
}