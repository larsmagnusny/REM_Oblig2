// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Inventory.h"

Inventory::Inventory(int32 NumSlots)
{
	// Sett InventoryStorage til å være en pointer til en array av InventoryItems
	InventoryStorage = new InventoryItem*[NumSlots];

	// Vi må vite om en slot er okkupert
	AvailableSlots = new bool[NumSlots];

	// Initialiser til å være null og åpen
	for (int32 i = 0; i < NumSlots; i++)
	{
		InventoryStorage[i] = nullptr;
		AvailableSlots[i] = true;
	}

	// Vi må vite hvor stor arrayen er
	InventorySize = NumSlots;
}

Inventory::~Inventory()
{
	
}

bool Inventory::AddItem(InventoryItem* item)
{
	// Hent en tiljengelig slot index
	int32 index = GetAvailableIndex();

	// Hvis det finnes en slot som er ledig
	if (index != -1)
	{
		// Kopier over peker adressen
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
	// Fjern Peker referansen
	if (SlotNum <= InventorySize - 1)
	{
		InventoryStorage[SlotNum] = nullptr;
		AvailableSlots[SlotNum] = true;
	}
}

void Inventory::DiscardItem(InventoryItem* Item)
{
	// Ikke implementert enda, fordi jeg ikke har fått bruk for det enda
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

void Inventory::LoadInventoryFromPersistent(FBufferArchive & BinaryData, MeshAndTextureLoader* ResourceLoaderRef)
{
	int32 size = 0;
	int32 InteractID;
	int32 ID;
	ItemIDs ItemID;
	FString ItemName;

	FMemoryReader Ar = FMemoryReader(BinaryData, true);

	Ar << size;

	for (int32 i = 0; i < size; i++)
	{
		Ar << InteractID;
		Ar << ID;
		Ar << ItemName;

		UE_LOG(LogTemp, Warning, TEXT("Loaded: %s"), *ItemName);

		if (ID != -1)
		{
			ItemID = (ItemIDs)ID;
			InventoryStorage[i] = new InventoryItem(ItemID, InteractID, ItemName, ResourceLoaderRef->GetStaticMeshByItemID(ItemID), ResourceLoaderRef->GetTextureByItemID(ItemID));
			AvailableSlots[i] = false;
		}
	}
}

void Inventory::SaveInventoryToPersistent(FBufferArchive & BinaryData)
{
	int32 size = 0;
	size = BinaryData.Num();

	// Size of inventory
	size = GetSize();
	BinaryData << size;

	for (int32 i = 0; i < size; i++)
	{
		int32 InteractID = 0;
		int32 ItemID = -1;
		FString ItemName = "ItemNone";

		if (InventoryStorage[i])
		{
			InteractID = InventoryStorage[i]->INTERACT_ID;
			ItemID = (int32)InventoryStorage[i]->ItemID;
			ItemName = InventoryStorage[i]->Name;
			
			UE_LOG(LogTemp, Warning, TEXT("Saved: %s"), *ItemName);
		}

		BinaryData << InteractID;
		BinaryData << ItemID;
		BinaryData << ItemName;
	}
}

int32 Inventory::GetSize()
{
	return InventorySize;
}
