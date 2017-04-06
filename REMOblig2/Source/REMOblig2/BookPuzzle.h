// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "BookPuzzle.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UBookPuzzle : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UBookPuzzle();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Solve Puzzle")
	void SolvePuzzle();

	virtual FBufferArchive GetSaveData() override;
	virtual void LoadSaveData(FMemoryReader& Ar) override;
	
private:
	UClass* PuzzleWidgetClassTemplate = nullptr;
	UUserWidget* PuzzleWidget = nullptr;

	bool PuzzleSolved = false;
	bool HasPuzzleBeenSolved = false;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;
};
