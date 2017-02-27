// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "StructsAndEnums.h"

/**
 * 
 */
class REMOBLIG2_API InventoryItem
{
public:
	InventoryItem(ItemIDs ID, int InteractID, FString NameToUse, UStaticMesh* MeshToUse, UTexture2D* Icon);
	~InventoryItem();

	// Enum that defines items
	ItemIDs ItemID;

	// The ID it has to interact with other objects
	int INTERACT_ID = 0;
	FString Name = "";
	UStaticMesh* Mesh = nullptr;
	UTexture2D* InventoryIcon = nullptr;
};
