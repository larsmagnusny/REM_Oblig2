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

	UPROPERTY(EditAnywhere)
	FVector CameraAngle = FVector(0.f, -60.f, 45.f);

	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);
};
