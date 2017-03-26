// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "Engine/StaticMeshActor.h"
#include "InventoryItem.h"
#include "InventoryItemComponent.h"
#include "InventoryItemObject.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AInventoryItemObject : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	AInventoryItemObject();
	~AInventoryItemObject();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Denne kj�res i deferred spawning for � kopiere over referanser som brukes p� dette objektet, mesh icon osv...
	void Init(InventoryItem* Item);

	// Denne brukes dersom vi ikke spawner i script men legger til objektet i verden i editoren
	// S� henter den tekstur og icon ifra MeshAndTextureLoader instansen
	UPROPERTY(EditAnywhere)
	ItemIDs ItemID;

	// N�r dette objektet brukes med et annet objekt s� bruker vi denne iden
	UPROPERTY(EditAnywhere)
	int INTERACT_ID = 0;

	// Navnet p� objektet...
	UPROPERTY(EditAnywhere)
	FString Name = "";

	// Brukes n�r vi legger til dette objektet i spiller-inventory
	InventoryItem* InvItemRef = nullptr;

	bool NeedDelete = false;
private:
	// Peker til komponenten vi lager
	UInventoryItemComponent* InventoryItemComponent = nullptr;

	// Mesh som brukes
	UStaticMesh* Mesh = nullptr;

	// Ikon den skal ha i inventoryen
	UTexture2D* InventoryIcon = nullptr;
};
