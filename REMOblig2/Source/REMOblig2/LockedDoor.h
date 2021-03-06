// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "InventoryItem.h"
#include "LockedDoor.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class OpenCondition : uint8
{
	OPEN_KEY UMETA(DisplayName="Door Opens with Key"),
	OPEN_COMPLETE_PUZZLE UMETA(DisplayName = "Door Opens with Completion of puzzle"),
	OPEN_TRIGGER UMETA(DisplayName = "Door Opens with TriggerVolume"),
	OPEN_NORMAL UMETA(DisplayName = "Door Opens no matter what")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API ULockedDoor : public UInteractableComponent
{
	GENERATED_BODY()
public:
	ULockedDoor();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	virtual void ItemInteract(int32 SlotNum) override;

	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;

	virtual FVector GetActivatePosition(AActor* Player) override;

	void SetPuzzleSolved();

	UPROPERTY(EditAnywhere)
	bool OpenDirection = false;

	UPROPERTY(EditAnywhere)
	int OpenID = 0;	// Defaults to none

	UPROPERTY(EditAnywhere)
	OpenCondition DoorOpenCondition;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* Trigger = nullptr;

	UPROPERTY(EditAnywhere)
	bool Open = false;	// Save this

	UPROPERTY(EditAnywhere)
	float MaxOpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 100.f;
private:
	void OpenDoor();
	void CloseDoor();

	bool PuzzleSolved = false; // Save this

	FRotator InitialRotation; // Save this
	float CurrentRotation = 0.f; // Save this

	AREM_GameMode* GameMode = nullptr;

	FVector OpenDir;
};
