// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "Camera/CameraActor.h"
#include "LookAtCamera.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API ALookAtCamera : public ACameraActor
{
	GENERATED_BODY()
public:
	ALookAtCamera();
	virtual void Tick(float DeltaTime) override;	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float KeepCameraStillInThisRadius = 70.f;

	UPROPERTY(EditAnywhere)
	bool MainCamera = false;

	UPROPERTY(EditAnywhere)
	bool FollowCharacter = false;

	UPROPERTY(EditAnywhere)
	bool FollowAlongAxisX = false;
	UPROPERTY(EditAnywhere)
	bool FollowAlongAxisY = false;
	UPROPERTY(EditAnywhere)
	bool FollowAlongAxisZ = false;

	UPROPERTY(EditAnywhere)
	float FollowDistance = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxFollowDistance = 0.f;

	UPROPERTY(EditAnywhere)
	bool LockX = false;

	UPROPERTY(EditAnywhere)
	bool LockY = false;

	UPROPERTY(EditAnywhere)
	bool LockZ = false;

	UPROPERTY(EditAnywhere)
	float MinPitch = 0.f;
	UPROPERTY(EditAnywhere)
	float MaxPitch = 0.f;

	UPROPERTY(EditAnywhere)
	float MinRoll = 0.f;
	UPROPERTY(EditAnywhere)
	float MaxRoll = 0.f;

	UPROPERTY(EditAnywhere)
	float MinYaw = 0.f;
	UPROPERTY(EditAnywhere)
	float MaxYaw = 0.f;

private:
	AREM_GameMode* GameMode = nullptr;
	ACharacter* LookAt = nullptr;

	FRotator OrigRotation;
	FVector OrigPosition;
};
