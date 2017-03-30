// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "MovieSceneFadeTrack.h"
#include "REM_Hud.h"
#include "Components/SceneComponent.h"
#include "FadeController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UFadeController : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFadeController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void FadeIn();

	void FadeOut(bool LoadAfterDone, FName LevelName);

	void StartAnimation();
	void StopAnimation();

	bool LoadLevelAfterFadeDone = false;
	FName LevelToLoad;

	float LastPosition = 0.f;

	ALevelSequenceActor* Parent = nullptr;

	ULevelSequence* FadeInSequence = nullptr;
	ULevelSequence* FadeOutSequence = nullptr;

	AREM_Hud* HudInstance = nullptr;
};
