// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Blueprint/UserWidget.h"
#include "InteractableObject.h"
#include "InteractableComponent.h"
#include "Inventory.h"
#include "ClimbableObject.h"
#include "MeshAndTextureLoader.h"
#include "REM_GameInstance.h"

#include "GameFramework/GameModeBase.h"
#include "REM_GameMode.generated.h"

struct InteractionWidget
{
	AActor* Owner;
	UUserWidget* MenuWidget;
	UInteractableComponent* ParentComponent;
	AInteractableObject* ParentObject;
};

//USTRUCT(Blueprintable)
struct InteractableObject
{
	//UPROPERTY(BlueprintReadWrite, Category = "InteractionWidget")
	AActor* OwningActor = nullptr; // The object that owns the component. If It's an AInteractableObject, then OwningActor and AInteractableObject is the same.
	//UPROPERTY(BlueprintReadWrite, Category = "InteractionWidget")
	UInteractableComponent* ScriptComponent = nullptr;	// Pointer to the attached class to the owning actor.
	//UPROPERTY(BlueprintReadWrite, Category = "InteractionWidget")
	AInteractableObject* StaticMeshInstance = nullptr;	// Same class as OwningActor if it is set.
};

UCLASS()
class REMOBLIG2_API AREM_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	// Constructor
	AREM_GameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Sets a pointer to the main camera so we can access it in other scripts.
	void SetMainCamera(UCameraComponent* Camera);

	// For when we implement hiding objects that block our view to the player
	void RayCastArray(FHitResult* Ray, FVector* Start, FVector* Direction, float LengthOfRay, int size, AActor* ActorToIgnore);

	// We need to keep track of our highlightable objects.
	void AddInteractableObject(AActor* Actor, UInteractableComponent* Component = nullptr, AInteractableObject* StaticMeshInstance = nullptr);
	void RemoveInteractableObject(AActor* Actor);
	bool IsInteractble(AActor* Actor);
	
	// We should be able to acess our highlightable objects from other classes
	UInteractableComponent* GetInteractor(AActor* Actor);
	AInteractableObject* GetStaticMeshInteractor(AActor* Actor);

	// Set a pointer to our MainCharacter so we can easily acess it everywhere
	void SetMainCharacter(ACharacter* Character);
	
	// We should be able to put an object in the world when it's dropped from an inventory, or if we want to spawn it in the world for some reason.
	void PutObjectInWorld(InventoryItem* Item, FVector Position, FVector Rotation, FVector Scale);

	// Get a pointer to the MainCharacter!
	UFUNCTION(BlueprintCallable, Category = "GetCharacter")
	ACharacter* GetMainCharacter();

	InteractableObject* GetInteractableObject(AActor* Actor);

	MeshAndTextureLoader* MeshesAndTextures;

	void SaveAllData();

	UFUNCTION()
	void SpawnMap(FName MapName);

	UFUNCTION()
	void UnloadMap(FName MapName);

	void SortArray(TArray<UInteractableComponent*>& Array);
	void GetRelevantSaveData(FBufferArchive &BinaryData);
	void LoadDataFromBinary(FBufferArchive &BinaryData);

	UFUNCTION(BlueprintCallable, Category = "Load All Data")
	FName LoadAllData();

	FName CurrentLoadedMap = "";

	bool LoadSave = true;

	float counter = 0.f;

	USceneComponent* FadeController = nullptr;
	bool FadeIn = true;
private:
	// Pointer to the main camera
	UCameraComponent* MainCamera = nullptr;

	// Keep track of items we are able to highlight with our mouse cursor
	TArray<InteractableObject> InteractableObjects;

	// Last component we highlighted
	InteractableObject* LastComponent = nullptr;

	// Pointer to our main character
	ACharacter* MainCharacter = nullptr;

	UREM_GameInstance* GameInstance = nullptr;

	REMSaveGame* SaveGameInstance;

	FString GlobalSaveFile = "CharacterData.bin";
};
