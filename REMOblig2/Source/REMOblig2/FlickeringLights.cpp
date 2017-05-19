// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "FlickeringLights.h"


// Sets default values for this component's properties
UFlickeringLights::UFlickeringLights()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlickeringLights::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PointLight = Cast<UPointLightComponent>(GetOwner()->GetComponentByClass(UPointLightComponent::StaticClass()));

	OriginalIntensity = PointLight->Intensity;
}


// Called every frame
void UFlickeringLights::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!CanFlicker)
		return;

	if (!isOn)
	{
		PointLight->SetIntensity(0);
		return;
	}

	if (FlickerOnEvent)
	{

	}
	else {
		if (RandomFlicker)
		{
			if (FlickerDuration < FlickerDurationCounter)
			{
				RandomFlicker = false;
				FlickerWait = FMath::RandRange(MinFlickerWait, MaxFlickerWait);
				FlickerDurationCounter = 0.f;
			}

			if (FlickerSpeed < TimeCounter)
			{
				TimeCounter = 0.f;
				float RandIntensity = FMath::RandRange(MinIntensity, MaxIntensity);
				PointLight->SetIntensity(RandIntensity);
			}
			FlickerDurationCounter += DeltaTime;
		}
		else if (TimeCounter > FlickerWait) {
			RandomFlicker = true;
			FlickerDurationCounter = 0.f;
			TimeCounter = 0.f;
		}

		if (!RandomFlicker) {
			if (PointLight->Intensity != OriginalIntensity)
			{
				PointLight->SetIntensity(OriginalIntensity);
			}
		}

		TimeCounter += DeltaTime;
	}
}

