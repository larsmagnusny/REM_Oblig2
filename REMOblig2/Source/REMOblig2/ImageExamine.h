// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "LookAtCamera.h"
#include "Book.h"
#include "InteractableComponent.h"
#include "ImageExamine.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UImageExamine : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UImageExamine();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void ActivateObject(AActor* Player) override;

	virtual FVector GetActivatePosition(AActor* Player) override;

	UPROPERTY(EditAnywhere)
	float TransitionTime = 1.f;

	UPROPERTY(EditAnywhere)
	float RotateMultiplier = 16.3f;

	UPROPERTY(EditAnywhere)
	FVector MainCameraFinalPosition;

	UPROPERTY(EditAnywhere)
	FRotator MainCameraFinalRotation;

	FVector MainCameraOrigPosition;
	FRotator MainCameraOrigRotation;

	APlayerCameraManager* CameraManager = nullptr;
	ALookAtCamera* CurrentCamera = nullptr;

private:
	bool CanRunAnimation = true;
	bool RunCameraAnimation = false;
	bool Forward = true;
	bool StopCameraAnimation = false;


	bool AnimationRunning = false;


	bool OrigPositionAndRotationSet = false;

	bool ResetFollowCharacter = false;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

	ACharacter* MCharacter = nullptr;

	FRotator CurrentRotationOffset;

	
};
