// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class REMOBLIG2_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Distance between to actors
	float GetDistanceBetweenActors(AActor* Actor1, AActor* Actor2);

	// Inventory Functions
	bool AddItemToInventory(InventoryItem* Item);

	void SwapInventoryElements(int32 index1, int32 index2);

	void DropItem(int32 SlotIndex, FVector2D WorldLocation);

	void DiscardItem(int32 SlotNum);
	void DiscardItem(InventoryItem* item);

	// Blueprint Callable Functions!
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FString GetInventoryTextureAt(int32 SlotNum);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySize();

	UFUNCTION(BlueprintCallable, Category = "Character Raycast")
	void SetCanRayCast(bool val);

	// Mouse Input 
	void MouseLeftClick();
	void MouseRightClick();

	UPROPERTY(EditAnywhere)
	UClass* AnimClass = nullptr;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	// Camera Boom:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom = nullptr;

	float Mass = 0.0f;
private:
	// This can be toggled when hovering over a UI element
	bool CanClickRayCast = true;

	// Mouse movement
	bool MouseMove = false;
	bool DelayActivate = false;

	// A struct containing what we clicked on and attached script instances
	InteractableObject DelayActivateObject;

	FVector MoveTo = FVector(0.f, 0.f, 0.f);

	// For checking if we need to change highlighting
	UStaticMeshComponent* LastComponentMousedOver = nullptr;

	// Pointers to other class instances we need in this script
	AREM_GameMode* GameMode = nullptr;

	// Player Inventory
	Inventory* PlayerInventory = nullptr;

	// Navigation system
	UNavigationSystem* NavSys;

	float lastDistance = 0.0f;

	bool DelayClimb = false;
	FVector ClimbTo;
};
