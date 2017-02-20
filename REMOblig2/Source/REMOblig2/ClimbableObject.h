// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "InteractableObject.h"
#include "ClimbableObject.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AClimbableObject : public AInteractableObject
{
	GENERATED_BODY()
public:
	AClimbableObject();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateObject(AActor* Player);
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* MeshToUse = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> MaterialsToUse;

	UPROPERTY(EditAnywhere)
	FVector Scale3D = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	FVector LedgePosition;
};
