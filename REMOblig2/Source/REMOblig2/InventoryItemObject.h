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

	// Denne kjøres i deferred spawning for å kopiere over referanser som brukes på dette objektet, mesh icon osv...
	void Init(InventoryItem* Item);

	// Denne brukes dersom vi ikke spawner i script men legger til objektet i verden i editoren
	// Så henter den tekstur og icon ifra MeshAndTextureLoader instansen
	UPROPERTY(EditAnywhere)
	ItemIDs ItemID;

	// Når dette objektet brukes med et annet objekt så bruker vi denne iden
	UPROPERTY(EditAnywhere)
	int INTERACT_ID = 0;

	// Navnet på objektet...
	UPROPERTY(EditAnywhere)
	FString Name = "";

	// Brukes når vi legger til dette objektet i spiller-inventory
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
