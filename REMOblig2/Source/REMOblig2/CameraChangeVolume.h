// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Engine/TriggerVolume.h"
#include "MainCharacter.h"
#include "CameraChangeVolume.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API ACameraChangeVolume : public ATriggerVolume
{
	GENERATED_BODY()
public:
	ACameraChangeVolume();

	virtual void BeginPlay() override;

	// Kamera vinkel som hoved kameraet skal settes til, kan endres i editoren
	UPROPERTY(EditAnywhere)
	FVector CameraAngle = FVector(0.f, -60.f, 45.f);
	
	UPROPERTY(EditAnywhere)
	AActor* CameraToSwitchTo = nullptr;

	// Sjekk om vi overlapper noe
	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);
};
