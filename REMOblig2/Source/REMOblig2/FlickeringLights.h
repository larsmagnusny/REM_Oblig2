// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Components/ActorComponent.h"
#include "FlickeringLights.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UFlickeringLights : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlickeringLights();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPointLightComponent* PointLight;

	UPROPERTY(EditAnywhere)
	bool CanFlicker = false;

	bool RandomFlicker = false;
	
	UPROPERTY(EditAnywhere)
	bool FlickerOnEvent = false;

	UPROPERTY(EditAnywhere)
	float MinIntensity = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxIntensity = 0.f;

	UPROPERTY(EditAnywhere)
	float FlickerSpeed = 0.f;	// How long to wait between change in intensity

	float TimeCounter = 0.f;
	float FlickerDurationCounter = 0.f;

	UPROPERTY(EditAnywhere)
	float FlickerDuration = 0.f;

	UPROPERTY(EditAnywhere)
	float MinFlickerWait = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxFlickerWait = 0.f;

	float FlickerWait = 0.f;

	float OriginalIntensity = 0.f;
};
