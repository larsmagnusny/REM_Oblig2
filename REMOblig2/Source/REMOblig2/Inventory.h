// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "InventoryItem.h"
/**
 * 
 */
class REMOBLIG2_API Inventory
{
public:
	Inventory(int32 NumSlots);
	~Inventory();

	// Add an item to the inventory
	bool AddItem(InventoryItem* item);

	// Remove an item from the inventory
	void DiscardItem(int32 SlotNum);
	void DiscardItem(InventoryItem* Item);

	// So that the HUD can load the icon to show in the menu for the item
	UTexture2D* GetTextureReference(int32 SlotNum);

	// Bytt om items fra en slot til en annen
	void Swap(int32 index1, int32 index2);

	// Hent en index som ikke allerede har en item
	int32 GetAvailableIndex();

	// Hent en item i den slotten som er definert
	InventoryItem* GetItem(int32 SlotNum);

	// Hent en item som har en itemID
	InventoryItem* GetItemById(ItemIDs ID);
private:
	// Pointer to a pointer...
	InventoryItem** InventoryStorage;
	bool* AvailableSlots;
	int32 InventorySize = 0;
};
