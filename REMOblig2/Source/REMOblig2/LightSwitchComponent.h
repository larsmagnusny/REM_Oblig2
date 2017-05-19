// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "REM_GameMode.h"
#include "LightSwitchComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API ULightSwitchComponent : public UInteractableComponent
{
	GENERATED_BODY()
public:
	ULightSwitchComponent();

	virtual void BeginPlay();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	virtual FVector GetActivatePosition(AActor* Player);


	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Lights;

	UPROPERTY(EditAnywhere)
	float OnIntensity = 1.f;
	
	UPROPERTY(EditAnywhere)
	float OffIntensity = 0.f;

	UPROPERTY(EditAnywhere)
	bool On = false;
private:
	
	bool LastState = true;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
