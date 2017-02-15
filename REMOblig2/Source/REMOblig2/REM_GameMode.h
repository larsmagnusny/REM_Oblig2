// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "InteractableComponent.h"
#include "GameFramework/GameModeBase.h"
#include "REM_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AREM_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	struct InteractableObject
	{
		AActor* OwningActor = nullptr; // The object that owns the component. If It's an AInteractableObject, then OwningActor and AInteractableObject is the same.
		UInteractableComponent* ScriptComponent = nullptr;	// Pointer to the attached class to the owning actor.
		AInteractableObject* StaticMeshInstance = nullptr;	// Same class as OwningActor if it is set.
	};

	// Constructor
	AREM_GameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Sets a pointer to the main camera so we can access it in other scripts.
	void SetMainCamera(UCameraComponent* Camera);

	// For when we implement hiding objects that block our view to the player
	void RayCastArray(FHitResult* Ray, int size);

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
	void PutObjectInWorld();

	// Get a pointer to the MainCharacter!
	UFUNCTION(BlueprintCallable, Category = "GetCharacter")
	ACharacter* GetMainCharacter();
private:
	// Pointer to the main camera
	UCameraComponent* MainCamera = nullptr;

	// Keep track of items we are able to highlight with our mouse cursor
	TArray<InteractableObject*> InteractableObjects;

	// Last component we highlighted
	InteractableObject* LastComponent = nullptr;

	// Pointer to our main character
	ACharacter* MainCharacter = nullptr;
};
