// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "InventoryItemComponent.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API UInventoryItemComponent : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	UInventoryItemComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;
	virtual void PickupObject(AActor* Player) override;
	
private:
	AREM_Hud* Hud = nullptr;
	AREM_GameMode* GameMode = nullptr;
	
};
