// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Inventory.h"

Inventory::Inventory(int32 NumSlots)
{
	InventoryStorage = new InventoryItem*[NumSlots];
	AvailableSlots = new bool[NumSlots];

	for (int32 i = 0; i < NumSlots; i++)
	{
		InventoryStorage[i] = nullptr;
		AvailableSlots[i] = true;
	}

	InventorySize = NumSlots;
}

Inventory::~Inventory()
{
	for (int32 i = 0; i < InventorySize; i++)
	{
		delete[] InventoryStorage[i];
		InventoryStorage[i] = nullptr;
	}

	delete[] InventoryStorage;
	InventoryStorage = nullptr;

	delete[] AvailableSlots;
}

bool Inventory::AddItem(InventoryItem* item)
{
	int32 index = GetAvailableIndex();

	if (index != -1)
	{
		InventoryStorage[index] = item;
		AvailableSlots[index] = false;
		return true;
	}
	else {
		return false;
	}
}


void Inventory::DiscardItem(int32 SlotNum)
{
	if (SlotNum <= InventorySize - 1)
	{
		InventoryStorage[SlotNum] = nullptr;
		AvailableSlots[SlotNum] = true;
	}
}

void Inventory::DiscardItem(InventoryItem* Item)
{

}

UTexture2D* Inventory::GetTextureReference(int32 SlotNum)
{
	if (InventoryStorage[SlotNum])
		return InventoryStorage[SlotNum]->InventoryIcon;
	else
		return nullptr;
}

void Inventory::Swap(int32 index1, int32 index2)
{

}

int32 Inventory::GetAvailableIndex()
{
	for (int32 i = 0; i < InventorySize; i++)
	{
		if (AvailableSlots[i])
			return i;
	}

	return -1;
}

InventoryItem* Inventory::GetItem(int32 SlotNum)
{
	if (SlotNum <= InventorySize - 1)
	{
		if (InventoryStorage[SlotNum])
			return InventoryStorage[SlotNum];
	}
	return nullptr;
}

InventoryItem* Inventory::GetItemById(ItemIDs ID)
{
	for (int32 i = 0; i < InventorySize; i++)
	{
		if (InventoryStorage[i])
		{
			if (InventoryStorage[i]->ItemID == ID)
				return InventoryStorage[i];
		}
	}

	return nullptr;
}