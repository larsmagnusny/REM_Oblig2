// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Components/ActorComponent.h"
#include "REM_GameMode.h"
#include "InteractableComponent.h"
#include "LevelChangeActor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API ULevelChangeActor : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelChangeActor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void ActivateObject(AActor* Player) override;
		
	UPROPERTY(EditAnywhere)
	FName LevelName;

	AREM_GameMode* GameMode = nullptr;

	UPROPERTY(EditAnywhere)
	FVector ConnectedPosition;
};
