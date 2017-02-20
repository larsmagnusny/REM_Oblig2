// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "InventoryItem.h"
#include "LockedDoor.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API ULockedDoor : public UInteractableComponent
{
	GENERATED_BODY()
public:
	ULockedDoor();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject() override;

	UPROPERTY(EditAnywhere)
	bool OpenDirection = false;
private:
	void OpenDoor();
	void CloseDoor();

	void UnlockDoor(InventoryItem* item);
	void LockDoor(InventoryItem* item);
};
