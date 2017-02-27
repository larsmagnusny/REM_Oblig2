// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "ChestController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UChestController : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChestController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;
	virtual void OpenInventory(AActor* Player) override;
	virtual void PickupObject(AActor* Player) override;
	virtual void ActivateDialogue(AActor* Player) override;
	virtual void DialogueOptionPressed(UUserWidget* Caller, int optionindex) override;
	virtual void ItemInteract(int32 SlotNum) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue Pressed")
	AActor* SlotToGoTo();

	UFUNCTION(BlueprintCallable, Category = "Dialogue Pressed")
	void SnapToSlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Is it open?")
	bool isOpen = false;

	UPROPERTY(EditAnywhere)
	bool FollowWhenActivate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FollowPlayer?")
	bool FollowingPlayer = false;

	UPROPERTY(EditAnywhere)
	bool locked = true;

	UPROPERTY(EditAnywhere)
	int OPENID = 1337;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player To follow")
	AActor* PlayerToFollow = nullptr;

	AREM_Hud* Hud = nullptr;

	AREM_GameMode* GameMode = nullptr;

	// A place where the chest can sit...
	UPROPERTY(EditAnywhere)
	AActor* Slot1 = nullptr;

	// A place where the chest can sit
	UPROPERTY(EditAnywhere)
	AActor* Slot2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Should i try to go to the slot?")
	bool GoToSlot = false;
	int Slot = -1;
	bool SnappedToSlot = false;
	FRotator Rotation;
	FVector SlotLocation;

	bool filled = false;
private:
	int toysfilled = 0;
};
