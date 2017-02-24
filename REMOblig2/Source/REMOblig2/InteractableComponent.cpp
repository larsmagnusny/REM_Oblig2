// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "InteractableComponent.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	
}

void UInteractableComponent::ActivateObject(AActor* Player)
{

}

UStaticMeshComponent* UInteractableComponent::GetStaticMeshComponent()
{
	return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

USkeletalMeshComponent* UInteractableComponent::GetSkeletalMeshComponent()
{
	return Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}

FVector UInteractableComponent::GetActivatePosition(AActor* Player)
{
	FVector ActorLocation = GetOwner()->GetActorLocation();

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	bool StaticMesh = false;

	if (MeshComponent)
		StaticMesh = true;

	
	FVector Bounds = FVector::ZeroVector;
	
	if (StaticMesh)
		Bounds = MeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
	else
		Bounds = SkeletalMeshComponent->Bounds.BoxExtent;

	if (Bounds == FVector::ZeroVector)
		return GetOwner()->GetActorLocation();

	FVector ActivatePosition = ActorLocation + GetOwner()->GetActorForwardVector()*(Bounds.X + 20);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(Bounds.X));

	DrawDebugSphere(GetWorld(), ActivatePosition, 10.f, 5, FColor(255, 0, 0, 255), true, 5.0f, 0, 1.0f);

	return ActivatePosition;
}