// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "InteractableComponent.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	
}

void UInteractableComponent::ActivateObject()
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

FVector UInteractableComponent::GetActivatePosition()
{
	return FVector(0.f, 0.f, 0.f);
}