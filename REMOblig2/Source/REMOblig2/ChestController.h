// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "ChestController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UChestController : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChestController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void ActivateObject() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Is it open?")
	bool isOpen = false;
private:
	
};
