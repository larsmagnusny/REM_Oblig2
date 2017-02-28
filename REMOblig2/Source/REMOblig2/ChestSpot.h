// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "ChestController.h"
#include "StructsAndEnums.h"
#include "ChestSpot.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UChestSpot : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UChestSpot();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	UPROPERTY(EditAnywhere)
	AActor* ActorToSnapToSlot;

	UPROPERTY(EditAnywhere)
	ItemIDs ItemToDrop;

	UPROPERTY(EditAnywhere)
	int OPEN_ID = 0;

	UPROPERTY(EditAnywhere)
	AActor* Chest = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* Door = nullptr;

	UPROPERTY(EditAnywhere)
	bool CanDropItem = false;

private:
	// Used to send messages to the chest...
	UChestController* ChestScriptInstance = nullptr;

	bool ItemDropped = false;

	bool PuzzleSolved = false;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
