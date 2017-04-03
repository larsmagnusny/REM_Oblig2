// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableComponent.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "Book.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REMOBLIG2_API UBook : public UInteractableComponent
{
	GENERATED_BODY()
public:
	UBook();
	
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateObject(AActor* Player) override;

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player) override;

	// Data to save about this object, can be overriden
	virtual FBufferArchive GetSaveData() override;

	// Data to load about this object, can be overriden
	virtual void LoadSaveData(FMemoryReader &Ar) override;

	void SetInteractable();
	void UnsetInteractable();

	void SetParent(UInteractableComponent* Parent);

	UFUNCTION()
	void BeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	FVector OriginalPosition;
	FRotator OriginalRotation;

	FVector CurrentPosition;
	
	int OccupyingIndex = 0;

	UPROPERTY(EditAnywhere)
	FString BookTitle = "";

	bool CanOverlap = true;

private:
	AREM_GameMode* GameMode = nullptr;
	AREM_Hud* Hud = nullptr;

	UInteractableComponent* ParentComponent = nullptr;

	

	float MoveCounter = 0.f;
	float Interval = 0.01f;

	bool Wait = false;
	float TimeToWait = 0.5f;
	float TimeCounter = 0.f;
};
