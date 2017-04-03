// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Components/ActorComponent.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "LookAtCamera.h"
#include "Book.h"
#include "InteractableComponent.h"
#include "BookCase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UBookCase : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBookCase();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;

	void MakeAllBooksInteractable();
	void MakeAllBooksNonInteractable();

	FVector GetPositionFromSlot(int Index);

	void SwapPositions(int index1, int index2);

	UBook* GetBookComponentByActor(AActor* BookObject);

	bool CheckSolution();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Books;

	UPROPERTY(EditAnywhere)
	FVector MainCameraFinalPosition;

	UPROPERTY(EditAnywhere)
	FRotator MainCameraFinalRotation;

	FVector MainCameraOrigPosition;
	FRotator MainCameraOrigRotation;

	APlayerCameraManager* CameraManager = nullptr;
	ALookAtCamera* CurrentCamera = nullptr;
	
	bool DraggingBook = false;
	AActor* BookToDrag = nullptr;

	UPROPERTY(EditAnywhere)
	ItemIDs ItemToDrop;
	UPROPERTY(EditAnywhere)
	int InteractID;

	UPROPERTY(EditAnywhere)
	FVector PositionToDrop;

private:
	bool CanRunAnimation = true;
	bool RunCameraAnimation = false;
	bool Forward = true;
	bool StopCameraAnimation = false;

	bool PuzzleSolved = false;

	bool OrigPositionAndRotationSet = false;

	FRotator CurrentRotationOffset;

	TArray<UBook*> BookComponents;
	TArray<FVector> SnapPositions;

	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

	ACharacter* MCharacter = nullptr;

	float CheckSolutionInterval = 1.f;
	float TimeCounter = 0.f;

	bool ShouldWait = false;
	float Wait = 2.f;

	bool LoadGameInit = true;
};
