// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "ToiletScript.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UToiletScript : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	UToiletScript();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	virtual FBufferArchive GetSaveData() override;
	virtual void LoadSaveData(FMemoryReader& Ar) override;

	UPROPERTY(EditAnywhere)
	ItemIDs ItemToAddToInventoryOnActivate;

	UPROPERTY(EditAnywhere)
	bool ItemDropsWhenItemInteracts = false;
private:
	bool ItemAddedToInventory = false;

	bool HasBeenExamined = false;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
