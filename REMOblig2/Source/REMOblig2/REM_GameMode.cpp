// Fill out your copyright notice in the Description page of Project Settings.

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "InventoryItemObject.h"
#include "REM_Hud.h"
#include "MainCharacter.h"
#include "FadeController.h"

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

	GameInstance = Cast<UREM_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SaveGameInstance = new REMSaveGame();

	FString LevelSaveFile = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	GlobalSaveFile = LevelSaveFile;

	// Trigger Beginplay in GameInstance...
	GameInstance->BeginPlay();

	GameInstance->LoadCheckpoint = true;

	// Load Inventory from the GameInstance if its not empty...
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(GameInstance->PersistentInventory->Num()));

	
}

void AREM_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (FadeController && FadeIn)
	{
		FadeIn = false;
		Cast<UFadeController>(FadeController)->FadeIn();
	}

	if (GameInstance->LoadCheckpoint)
	{
		GameInstance->LoadCheckpoint = false;
		if (GameInstance->PersistentInventory)
		{
			if (GameInstance->PersistentInventory->Num() != 0)
			{
				if (MainCharacter)
					Cast<AMainCharacter>(MainCharacter)->ReloadInventory(*GameInstance->PersistentInventory, MeshesAndTextures);
				UE_LOG(LogTemp, Warning, TEXT("Reloaded Inventory"));
			}
		}


		if (GameInstance->LevelData[GameInstance->CurrentLevelLoaded])
		{
			if (GameInstance->LevelData[GameInstance->CurrentLevelLoaded]->Num() != 0)
			{
				LoadDataFromBinary(*GameInstance->LevelData[GameInstance->CurrentLevelLoaded]);
				UE_LOG(LogTemp, Warning, TEXT("Loaded LevelData"));
			}
		}

		if(!GameInstance->MainMenu)
			GameInstance->SaveAllData(SaveGameInstance);
	}
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
	ArrayInUse = true;
	InteractableObjects.Add(Object);
	ArrayInUse = false;
}

void AREM_GameMode::RemoveInteractableObject(AActor* Actor)
{
	ArrayInUse = true;
	for (int32 i = 0; i < InteractableObjects.Num(); i++)
	{
		if (InteractableObjects[i].OwningActor == Actor)
		{
			InteractableObjects.RemoveAt(i);
			ArrayInUse = false;
			return;
		}
	}
	ArrayInUse = false;
}

bool AREM_GameMode::IsInteractble(AActor* Actor)
{
	if (ArrayInUse)
		return false;

	for (InteractableObject obj : InteractableObjects)
	{
		if (ArrayInUse)
			return false;

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

void AREM_GameMode::SortArray(TArray<UInteractableComponent*>& Array)
{
	Array.Sort([&](UInteractableComponent& A, UInteractableComponent& B)
	{
		const FString& AName = A.ParentName;
		const FString& BName = B.ParentName;

		return AName.Compare(BName) < 0;
	});
}

void AREM_GameMode::GetRelevantSaveData(FBufferArchive &BinaryData)
{
	AMainCharacter* Char = Cast<AMainCharacter>(MainCharacter);
	
	// Save last location of player in this level
	FVector CharacterLocation = Char->GetActorLocation();
	FRotator CharacterRotation = Char->GetActorRotation();
	BinaryData << CharacterLocation;
	BinaryData << CharacterRotation;

	TArray<AInventoryItemObject*> DynamicObjects;
	// Save all inventory items that are on the ground...

	for (TActorIterator<AStaticMeshActor> Itr(GetWorld()); Itr; ++Itr)
	{
		AStaticMeshActor* Object = *Itr;


		if (Object->IsA(AInventoryItemObject::StaticClass()))
		{
			DynamicObjects.Add(Cast<AInventoryItemObject>(Object));
		}
	}



	int32 size = DynamicObjects.Num();
	BinaryData << size;

	for (int i = 0; i < size; i++)
	{
		int32 InteractID = DynamicObjects[i]->INTERACT_ID;
		int32 ID = (int32)DynamicObjects[i]->ItemID;
		FString Name = DynamicObjects[i]->Name;
		FVector Location = DynamicObjects[i]->GetActorLocation();
		FRotator Rotation = DynamicObjects[i]->GetActorRotation();

		BinaryData << InteractID;
		BinaryData << ID;
		BinaryData << Name;
		BinaryData << Location;
		BinaryData << Rotation;
	}

	TArray<UInteractableComponent*> InteractableComponents;
	for (TObjectIterator<UInteractableComponent> Itr; Itr; ++Itr)
	{
		if (*Itr)
		{
			InteractableComponents.Add(*Itr);
			(*Itr)->ParentName = (*Itr)->GetOwner()->GetName();
		}
	}

	SortArray(InteractableComponents);

	size = InteractableComponents.Num();

	for (int i = 0; i < size; i++)
	{
		FBufferArchive ObjectData = InteractableComponents[i]->GetSaveData();

		BinaryData.Append(ObjectData);
	}

	// Get all interactable components that exist in the scene
}

void AREM_GameMode::LoadDataFromBinary(FBufferArchive & BinaryData)
{
	AMainCharacter* Char = Cast<AMainCharacter>(MainCharacter);

	int32 size = 0;
	FVector CharacterLocation;
	FRotator CharacterRotation;

	FMemoryReader Ar = FMemoryReader(BinaryData, false);
	Ar.Seek(0);

	Ar << CharacterLocation;
	Ar << CharacterRotation;

	Char->SetActorLocation(CharacterLocation);
	
	CharacterRotation.Yaw += 180;
	Char->SetActorRotation(CharacterRotation);

	for (TActorIterator<AStaticMeshActor> Itr(GetWorld()); Itr; ++Itr)
	{
		AStaticMeshActor* Object = *Itr;
		

		if (Object->IsA(AInventoryItemObject::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Going through already existing items!"));
			Object->Destroy();
		}
	}

	Ar << size;

	for (int32 i = 0; i < size; i++)
	{
		int32 InteractID;
		int32 ID;
		FString Name;
		FVector Location;
		FVector Rotation;

		Ar << InteractID;
		Ar << ID;
		Ar << Name;
		Ar << Location;
		Ar << Rotation;

		ItemIDs ItemID = (ItemIDs)ID;

		InventoryItem* Item = new InventoryItem(ItemID, InteractID, Name, MeshesAndTextures->GetStaticMeshByItemID(ItemID), MeshesAndTextures->GetTextureByItemID(ItemID));

		PutObjectInWorld(Item, Location, Rotation, FVector(1, 1, 1));
	}

	TArray<UInteractableComponent*> InteractableComponents;
	for (TObjectIterator<UInteractableComponent> Itr; Itr; ++Itr)
	{
		if (*Itr)
		{
			InteractableComponents.Add(*Itr);
			(*Itr)->ParentName = (*Itr)->GetOwner()->GetName();
		}
	}

	SortArray(InteractableComponents);

	for (int32 i = 0; i < InteractableComponents.Num(); i++)
	{
		InteractableComponents[i]->LoadSaveData(Ar);
	}
}

FName AREM_GameMode::LoadAllData()
{
	FName LastLevel;
	GameInstance->LoadAllData(LastLevel, SaveGameInstance);
	return LastLevel;
}

void AREM_GameMode::SaveAllData()
{
	GameInstance->DeleteLevelData(GameInstance->CurrentLevelLoaded);
	GetRelevantSaveData(*GameInstance->LevelData[GameInstance->CurrentLevelLoaded]);

	GameInstance->DeletePersistentInventory();
	Cast<AMainCharacter>(MainCharacter)->SaveInventory(*GameInstance->PersistentInventory);
}

void AREM_GameMode::SpawnMap(FName MapName)
{
	SaveAllData();

	if (FadeController)
	{
		Cast<UFadeController>(FadeController)->FadeOut(true, MapName);
	}

	UE_LOG(LogTemp, Warning, TEXT("All Data Saved"));
	
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