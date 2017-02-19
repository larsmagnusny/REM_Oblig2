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

void AREM_GameMode::RayCastArray(FHitResult* Ray, int size)
{

}

void AREM_GameMode::SetMainCamera(UCameraComponent* Camera)
{
	MainCamera = Camera;
}

void AREM_GameMode::AddInteractableObject(AActor* Actor, UInteractableComponent* Component, AInteractableObject* StaticMeshInstance)
{

}

void AREM_GameMode::RemoveInteractableObject(AActor* Actor)
{

}

bool AREM_GameMode::IsInteractble(AActor* Actor)
{
	return true;
}

UInteractableComponent* AREM_GameMode::GetInteractor(AActor* Actor)
{
	return nullptr;
}

AInteractableObject* AREM_GameMode::GetStaticMeshInteractor(AActor* Actor)
{
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