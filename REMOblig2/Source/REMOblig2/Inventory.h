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
	FString GetTextureReference(int32 SlotNum);

	void Swap(int32 index1, int32 index2);

	int32 GetAvailableIndex();

	InventoryItem* GetItem(int32 SlotNum);
private:
	// Pointer to a pointer...
	InventoryItem** InventoryStorage;
	bool* AvailableSlots;
	int32 InventorySize = 0;
};
