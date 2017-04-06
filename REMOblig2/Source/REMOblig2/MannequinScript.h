// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "MannequinScript.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UMannequinScript : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UMannequinScript();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;
	virtual void ActivateDialogue(AActor* Player) override;
	virtual void ItemInteract(int32 SlotNum) override;

	virtual FBufferArchive GetSaveData() override;
	virtual void LoadSaveData(FMemoryReader& Ar) override;

	UPROPERTY(EditAnywhere)
	bool ItemDropsWhenItemInteracts = false;

	UPROPERTY(EditAnywhere)
	ItemIDs ItemToDrop;

	// Item you need to interact with this object...
	UPROPERTY(EditAnywhere)
	int InteractID = 911375;


private:
	bool ItemHasDropped = false;

	bool HasBeenExamined = false;
	
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

};
