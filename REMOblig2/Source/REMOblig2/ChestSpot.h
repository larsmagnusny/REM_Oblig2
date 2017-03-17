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

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	// Den itemen vi skal slippe dersom en kiste st�r p� den
	UPROPERTY(EditAnywhere)
	ItemIDs ItemToDrop;

	// Den INTERACT_IDen som itemet som blir sluppet har
	UPROPERTY(EditAnywhere)
	int OPEN_ID = 0;

	// Kisten som kan sitte p� denne
	UPROPERTY(EditAnywhere)
	AActor* Chest = nullptr;

	// D�ren som denne kan �pne
	UPROPERTY(EditAnywhere)
	AActor* Door = nullptr;

	// At den faktisk dropper en item n�r en kiste st�r p� den.
	UPROPERTY(EditAnywhere)
	bool CanDropItem = false;

private:
	// Used to send messages to the chest...
	UChestController* ChestScriptInstance = nullptr;

	// S� vi ikke slipper mer enn en item
	bool ItemDropped = false;

	// For � vite om puzzelet er l�st eller ikke
	bool PuzzleSolved = false;

	// Pekere til andre klasser
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
