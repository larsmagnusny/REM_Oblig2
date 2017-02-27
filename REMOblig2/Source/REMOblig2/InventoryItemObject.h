// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "Engine/StaticMeshActor.h"
#include "InventoryItem.h"
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
	virtual void Tick() override;

	void Init(InventoryItem* Item);

	UPROPERTY(EditAnywhere)
	ItemIDs ItemID;

	UPROPERTY(EditAnywhere)
	int INTERACT_ID = 0;

	UPROPERTY(EditAnywhere)
	FString Name = "";
private:
	InventoryItem* InvItemRef = nullptr;

	UStaticMesh* Mesh = nullptr;
	UTexture2D* InventoryIcon = nullptr;
};
