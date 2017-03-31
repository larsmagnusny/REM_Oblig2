// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#pragma once

#include "Components/ActorComponent.h"
#include "MenuIconsDef.h"
#include "InventoryItem.h"
#include "StructsAndEnums.h"
#include "InteractableComponent.generated.h"

enum ButtonTypes
{
	EXAMINE,
	OPEN,
	PICKUP,
	USE,
	DIALOGUE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	// This function all interactable components inherit
	// This can be triggered from anywhere
	virtual void ActivateObject(AActor* Player);
	virtual void ExamineObject(AActor* Player);
	virtual void PickupObject(AActor* Player);
	virtual void OpenInventory(AActor* Player);
	virtual void ActivateDialogue(AActor* Player);

	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData();

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar);

	// Item interacts with the object, when dropping an item from inventory to the object you want to interact with.
	UFUNCTION(BlueprintCallable, Category = "Item Interact with object...")
	virtual void ItemInteract(int32 SlotNum);

	// Base function inherited and overriden later
	UFUNCTION(BlueprintCallable, Category = "Dialogue Pressed")
	virtual void DialogueOptionPressed(UUserWidget* Caller, int optionindex);

	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player);

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimation = false;

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool canPlayerClick = true;

	// Used in Blueprint
	UFUNCTION(BlueprintCallable, Category = "MenuButtonArray")
	int GetNumMenuButtons();

	// Used in Blueprint
	UFUNCTION(BlueprintCallable, Category = "MenuButtonArray")
	TArray<UTexture2D*> GetMenuButtonTextures(int i);

	// Used in Blueprint
	UFUNCTION(BlueprintCallable, Category = "MenuButtonArray")
	ActionType GetActionType(int i);

	TArray<UMenuIconsDef*> MenuButtons;
	TArray<UMenuIconsDef*> ObjectSpecificMenuButtons;

	// Used in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuButtonArray")
	TArray<ActionType> Actions;

	// Used in Inherited Classes
	UClass* SubMenuWidgetClassTemplate = nullptr;
	UUserWidget* SubMenuWidget = nullptr;
private:
};
