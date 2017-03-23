// Fill out your copyright notice in the Description page of Project Settings.

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "InventoryItemObject.h"
#include "REM_Hud.h"

AREM_GameMode::AREM_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;	// Tell Unreal that this class can use the tick function every frame.
	DefaultPawnClass = nullptr;				// Not using the default pawn this game

	// We are using our custom class to control our HUD
	HUDClass = AREM_Hud::StaticClass();

	MeshesAndTextures = new MeshAndTextureLoader();
}

void AREM_GameMode::BeginPlay()
{
	Super::BeginPlay();

	SaveGameInstance = Cast<UREMSaveGame>(UGameplayStatics::CreateSaveGameObject(UREMSaveGame::StaticClass()));
	//UGameplayStatics::LoadGameFromSlot(SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(SaveGameInstance->DoorIndex));
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
	for (int32 i = 0; i < InteractableObjects.Num(); i++)
	{
		if (InteractableObjects[i].OwningActor == Actor)
		{
			InteractableObjects.RemoveAt(i);
			return;
		}
	}
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

void AREM_GameMode::UnloadMap(FName MapName)
{
	
}

void AREM_GameMode::SpawnMap(FName MapName)
{
	//SaveGameInstance->PlayerName = "TestPlayer";
	//SaveGameInstance->DoorIndex = 1234;

	//UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);

	UGameplayStatics::OpenLevel(GetWorld(), MapName);
}

void AREM_GameMode::SetMainCharacter(ACharacter* Character)
{
	MainCharacter = Character;
}

void AREM_GameMode::PutObjectInWorld(InventoryItem* Item, FVector Position, FVector Rotation, FVector Scale)
{
	FTransform t;
	t.SetLocation(Position);
	t.SetScale3D(Scale);
	t.SetRotation(Rotation.ToOrientationQuat());


	AInventoryItemObject* Object = Cast<AInventoryItemObject>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), AInventoryItemObject::StaticClass(), t));
	Object->Init(Item);
	Object->FinishSpawning(t);

}

ACharacter* AREM_GameMode::GetMainCharacter()
{
	return MainCharacter;
}