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

	// Vi kan bare aktivere og eksaminere dette objektet
	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	// Den itemen vi skal slippe dersom en kiste står på den
	UPROPERTY(EditAnywhere)
	ItemIDs ItemToDrop;

	// Den INTERACT_IDen som itemet som blir sluppet har
	UPROPERTY(EditAnywhere)
	int OPEN_ID = 0;

	// Kisten som kan sitte på denne
	UPROPERTY(EditAnywhere)
	AActor* Chest = nullptr;

	// Døren som denne kan åpne
	UPROPERTY(EditAnywhere)
	AActor* Door = nullptr;

	// At den faktisk dropper en item når en kiste står på den.
	UPROPERTY(EditAnywhere)
	bool CanDropItem = false;	// Save this

private:
	// Used to send messages to the chest...
	UChestController* ChestScriptInstance = nullptr;

	// Så vi ikke slipper mer enn en item
	bool ItemDropped = false; // Save this

	// For å vite om puzzelet er løst eller ikke
	bool PuzzleSolved = false;	// Save this

	// Pekere til andre klasser
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
