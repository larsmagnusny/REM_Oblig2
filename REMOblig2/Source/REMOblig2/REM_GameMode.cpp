// Fill out your copyright notice in the Description page of Project Settings.

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"

AREM_GameMode::AREM_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;	// Tell Unreal that this class can use the tick function every frame.
	DefaultPawnClass = nullptr;				// Not using the default pawn this game

	// We are using our custom class to control our HUD
	HUDClass = AREM_Hud::StaticClass();
}

void AREM_GameMode::BeginPlay()
{
	Super::BeginPlay();
}
void AREM_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AREM_GameMode::RayCastArray(FHitResult* Ray, FVector* Start, FVector* Direction, float LengthOfRay, int size, AActor* ActorToIgnore)
{
	for (int32 i = 0; i < size; i++)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(ActorToIgnore);
		GetWorld()->LineTraceSingleByChannel(Ray[i], Start[i], Start[i] + (Direction[i] * LengthOfRay), ECollisionChannel::ECC_WorldStatic, Params);
	}
}

void AREM_GameMode::SetMainCamera(UCameraComponent* Camera)
{
	MainCamera = Camera;
}

void AREM_GameMode::AddInteractableObject(AActor* Actor, UInteractableComponent* Component, AInteractableObject* StaticMeshInstance)
{
	InteractableObject Object;
	Object.OwningActor = Actor;
	Object.ScriptComponent = Component;
	Object.StaticMeshInstance = StaticMeshInstance;

	InteractableObjects.Add(Object);
}

void AREM_GameMode::RemoveInteractableObject(AActor* Actor)
{

}

bool AREM_GameMode::IsInteractble(AActor* Actor)
{
	for (InteractableObject obj : InteractableObjects)
	{
		if (obj.OwningActor == Actor)
			return true;
	}

	return false;
}

UInteractableComponent* AREM_GameMode::GetInteractor(AActor* Actor)
{
	for (InteractableObject obj : InteractableObjects)
	{
		if (obj.OwningActor == Actor)
			return obj.ScriptComponent;
	}

	return nullptr;
}

AInteractableObject* AREM_GameMode::GetStaticMeshInteractor(AActor* Actor)
{
	for (InteractableObject obj : InteractableObjects)
	{
		if (obj.OwningActor == Actor)
			return obj.StaticMeshInstance;
	}

	return nullptr;
}

InteractableObject* AREM_GameMode::GetInteractableObject(AActor* Actor)
{
	for (int32 i = 0; i < InteractableObjects.Num(); i++)
	{
		if (InteractableObjects[i].OwningActor == Actor)
			return &InteractableObjects[i];
	}

	return nullptr;
}

void AREM_GameMode::SetMainCharacter(ACharacter* Character)
{

}

void AREM_GameMode::PutObjectInWorld()
{

}

ACharacter* AREM_GameMode::GetMainCharacter()
{
	return MainCharacter;
}