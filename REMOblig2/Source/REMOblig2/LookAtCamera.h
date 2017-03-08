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
private:
	AREM_GameMode* GameMode = nullptr;
	ACharacter* LookAt = nullptr;
};
