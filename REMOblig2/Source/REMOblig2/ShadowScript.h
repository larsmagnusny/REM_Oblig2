// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "ShadowScript.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UShadowScript : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UShadowScript();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;


	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	ATriggerVolume* Trigger;
	
	UPROPERTY(EditAnywhere)
	FVector FinalPosition;
	
	UPROPERTY(EditAnywhere)
	FRotator FinalRotation;

	UPROPERTY(EditAnywhere)
	float Speed = 200.f;

	// How long to keep character scared...
	bool StartScareCounter = false;
	float ScareTimer = 1.5f;
	float ScareCounter = 0.f;
private:
	bool Animating = false;
	bool TriggerDone = false;
	bool CanEverTrigger = true;

	FVector CurrentPosition;
	FVector OriginalPosition;
};
