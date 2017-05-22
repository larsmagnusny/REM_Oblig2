// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "GameFramework/Actor.h"
#include "REM_GameInstance.h"
#include "REM_Hud.h"
#include "REM_GameMode.h"
#include "LookAtCamera.h"
#include "ChestController.h"
#include "CreditsController.generated.h"

UCLASS()
class REMOBLIG2_API ACreditsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreditsController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	float TransitionTime = 2.0f;

	UPROPERTY(EditAnywhere)
	float WaitTime = 3.f;

	float WaitCounter = 6.0f;

	UPROPERTY(EditAnywhere)
	TArray<FVector> CameraPositions;

	UPROPERTY(EditAnywhere)
	TArray<FRotator> CameraRotations;

	UPROPERTY(EditAnywhere)
	float RotateMultiplier = 16.3f;

	FVector MainCameraFinalPosition;
	FRotator MainCameraFinalRotation;

	FVector MainCameraOrigPosition;
	FRotator MainCameraOrigRotation;

	APlayerCameraManager* CameraManager = nullptr;
	ALookAtCamera* CurrentCamera = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Should show next credit...")
	bool ShouldShowNext = false;

	UPROPERTY(EditAnywhere)
	AActor* ChestActor = nullptr;
private:
	UREM_GameInstance* GameInstance = nullptr;
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
	ACharacter* MainCharacter = nullptr;

	bool RunCameraAnimation = false;

	bool AnimationRunning = false;

	ACharacter* MCharacter = nullptr;

	FRotator CurrentRotationOffset;

	bool CreditsDone = false;

	int CameraCounter = 0;

	
};
