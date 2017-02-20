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
	return false;
}


void Inventory::DiscardItem(int32 SlotNum)
{

}

void Inventory::DiscardItem(InventoryItem* Item)
{

}

FString Inventory::GetTextureReference(int32 SlotNum)
{
	return "";
}

void Inventory::Swap(int32 index1, int32 index2)
{

}

int32 Inventory::GetAvailableIndex()
{
	return InventorySize;
}

InventoryItem* Inventory::GetItem(int32 SlotNum)
{
	return nullptr;
}

InventoryItem* Inventory::GetItemById(int32 ID)
{
	for (int32 i = 0; i < InventorySize; i++)
	{
		if (InventoryStorage[i])
		{
			if (InventoryStorage[i]->ITEM_ID == ID)
				return InventoryStorage[i];
		}
	}

	return nullptr;
}