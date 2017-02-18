// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "ClimbableObject.h"

AClimbableObject::AClimbableObject()
{
	if (MeshToUse)
	{
		UStaticMeshComponent* Component = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

		Component->SetStaticMesh(MeshToUse);

		Component->SetRelativeScale3D(Scale3D);

		for (int32 i = 0; i < MaterialsToUse.Num(); i++)
		{
			Component->SetMaterial(i, MaterialsToUse[i]);
		}
	}
}

void AClimbableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AClimbableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClimbableObject::ActivateObject(AActor* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Climbable Wall has been activated!"));
}