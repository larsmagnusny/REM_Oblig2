// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "HideTrigger.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API UHideTrigger : public UInteractableComponent
{
	GENERATED_BODY()
private:
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

public:
	UHideTrigger();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;

};
