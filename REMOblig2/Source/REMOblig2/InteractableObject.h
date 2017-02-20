// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Engine/StaticMeshActor.h"
#include "InteractableObject.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AInteractableObject : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	AInteractableObject();

	// Activate this object, and send a reference to the player that activated it
	virtual void ActivateObject(AActor* Player);
};
