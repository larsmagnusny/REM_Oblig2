// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "InventoryItem.h"

InventoryItem::InventoryItem(ItemIDs ID, int InteractID, FString NameToUse, UStaticMesh* MeshToUse, UTexture2D* Icon)
{
	ItemID = ID;
	INTERACT_ID = InteractID;
	Name = NameToUse;
	Mesh = MeshToUse;
	InventoryIcon = Icon;
}

InventoryItem::~InventoryItem()
{
}
