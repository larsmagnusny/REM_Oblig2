<<<<<<< HEAD
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
=======
// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Components/ActorComponent.h"
#include "InteractableObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UInteractableObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableObject();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
>>>>>>> 3f4b62b2900fe2b404f68c2ba0b2660ccffac8cb
