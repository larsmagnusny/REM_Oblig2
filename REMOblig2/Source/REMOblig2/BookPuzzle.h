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
	
	void CreatePossibleSolution(int size);

	bool ExistsAnyOtherInRow(int column, int value, int AllowedOccurances);
	bool ExistsAnyOtherInColumn(int row, int value, int AllowedOccurances);

	bool ExistsAnyOtherInRow(int column, int value, int AllowedOccurances, int** ArrayToCheck);
	bool ExistsAnyOtherInColumn(int row, int value, int AllowedOccurances, int** ArrayToCheck);

	UFUNCTION(BlueprintCallable, Category = "Random Slot")
	void RandomSlotToShow(int& column, int& row, int size);

	UFUNCTION(BlueprintCallable, Category = "Check solution to puzzle")
	bool CheckSolution(int size, TArray<int> Array);

	UFUNCTION(BlueprintCallable, Category = "Get Value from slot")
	int GetSlotValueFromGrid(int column, int row);

	UFUNCTION(BlueprintCallable, Category = "Conversion")
	void FromRowAndColumnToIndex(int& index, int column, int row, int size);
	UFUNCTION(BlueprintCallable, Category = "Conversion")
	void FromIndexToRowAndColumn(int index, int& column, int& row, int size);
	
private:
	UClass* PuzzleWidgetClassTemplate = nullptr;
	UUserWidget* PuzzleWidget = nullptr;

	bool PuzzleSolved = false;
	bool HasPuzzleBeenSolved = false;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

	int** PossibleSolution;
	int size;
};
