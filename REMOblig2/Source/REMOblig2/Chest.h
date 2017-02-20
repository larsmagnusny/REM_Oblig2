// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableObject.h"
#include "Chest.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AChest : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	AChest();
	virtual void BeginPlay() override;
	virtual void ActivateObject(AActor* Player) override;

	
private:
	
	
};
