// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
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
	InventoryItem* GetItemByID(ItemIDs ID);
	InventoryItem* GetItemBySlot(int32 SlotNum);

	void SwapInventoryElements(int32 index1, int32 index2);

	void DropItem(int32 SlotIndex, FVector2D WorldLocation);

	void DiscardItem(int32 SlotNum);
	void DiscardItem(InventoryItem* item);

	// Change camera view
	void ChangeCameraView(FVector Vector);

	// For setting the dialogue options the player has when interacting with an object
	void SetDialogueOptions(TArray<FString> Options, UInteractableComponent* Caller);

	void SetDialogueChoiceVisible();
	void SetDialogueChoiceInvisible();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	FString GetDialogueOption(int i);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UInteractableComponent* GetTalkingTo();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool ShouldReloadDialogues = false;

	// Blueprint Callable Functions!
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UTexture2D* GetInventoryTextureAt(int32 SlotNum);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySize();

	UFUNCTION(BlueprintCallable, Category = "Character Raycast")
	void SetCanRayCast(bool val);

	// Mouse Input 
	void MouseLeftClick();
	void MouseRightClick();

	// Keyboard input
	void SpaceBarPressed();
	void SpaceBarReleased();

	UPROPERTY(EditAnywhere)
	UClass* AnimClass = nullptr;

	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UMaterial* CamoMaterial;

	UMaterial* StandardMaterial;

	// Camera Boom:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom = nullptr;

	float Mass = 0.0f;
private:
	TArray<FString> CurrentDialogueOptions;

	// This can be toggled when hovering over a UI element
	bool CanClickRayCast = true;

	// Mouse movement
	bool MouseMove = false;
	bool DelayActivate = false;
	bool DestinationReached = false;

	// HidingMovement
	bool SpaceBarDown = false;
	bool Hiding = false;
	FVector HideNormal = FVector(0, 0, 0);

	// A struct containing what we clicked on and attached script instances
	InteractableObject DelayActivateObject;

	FVector MoveTo = FVector(0.f, 0.f, 0.f);
	FVector ActivatePosition = FVector(0.f, 0.f, 0.f);

	// For checking if we need to change highlighting
	UStaticMeshComponent* LastComponentMousedOver = nullptr;
	USkeletalMeshComponent* LastSkeletalMeshComponentMousedOver = nullptr;

	// Pointers to other class instances we need in this script
	AREM_GameMode* GameMode = nullptr;

	// Player Inventory
	Inventory* PlayerInventory = nullptr;

	// Navigation system
	UNavigationSystem* NavSys;

	float lastDistance = 0.0f;
	int32 lastDistanceCounter = 0;


	bool DelayClimb = false;
	FVector ClimbTo;

	AREM_Hud* OurHud = nullptr;

	UClass* DialogueWidgetClassTemplate = nullptr;
	UUserWidget* DialogueWidget = nullptr;

	UInteractableComponent* TalkingTo = nullptr;
};
