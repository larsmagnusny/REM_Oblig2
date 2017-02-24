// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"

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

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	UPROPERTY(EditAnywhere)
	AActor* ActorToSnapToSlot;

private:
	// Used to send messages to the chest...
	UInteractableComponent* ChestScriptInstance = nullptr;
};
