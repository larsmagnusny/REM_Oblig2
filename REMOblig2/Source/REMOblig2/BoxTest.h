// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "BoxTest.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UBoxTest : public UInteractableComponent
{
	GENERATED_BODY()
private:
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

	

public:
	UBoxTest();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;

	virtual void ItemInteract(int32 SlotNum) override;

	virtual void ExamineObject(AActor* Player) override;
	
	
};
