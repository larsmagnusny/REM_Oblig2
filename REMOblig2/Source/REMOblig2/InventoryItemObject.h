// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "Engine/StaticMeshActor.h"
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
};
