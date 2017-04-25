// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

/**
 * 
 */

UENUM(BlueprintType)
enum class Sounds : uint8
{
	SOUND_CREEPY UMETA(DisplayName = "Creepy Sound"),
	SOUND_IDUNNO UMETA(DisplayName = "I don't know"),
	SOUND_MYSTERIOUS UMETA(DisplayName = "Mysterious Sound"),
	SOUND_OBJECTHIT1 UMETA(DisplayName = "Object Hit 1"),
	SOUND_OBJECTHIT2 UMETA(DisplayName = "Object Hit 2"),
	SOUND_OBJECTHIT3 UMETA(DisplayName = "Object Hit 3"),
	SOUND_OBJECTHIT4 UMETA(DisplayName = "Object Hit 4"),
	SOUND_OBJECTHIT5 UMETA(DisplayName = "Object Hit 5")
};

UENUM(BlueprintType)
enum class ItemIDs : uint8
{
	ITEM_KEY UMETA(DisplayName = "Key to open a door"),
	ITEM_TOY UMETA(DisplayName = "Just a toy"),
	ITEM_SPINNER UMETA(DisplayName = "Spinner Toy"),
	ITEM_PLIERS UMETA(DisplayName = "Pliers that can cut things"),
	ITEM_FEATHER UMETA(DisplayName = "A simple feather to tickle the chest."),
	ITEM_CRYSTALSHARD UMETA(DisplayName = "A Crystal shard"),
	ITEM_SCISSORS UMETA(DisplayName = "Scissors, do i have to say anything else?"),
	ITEM_NONE UMETA(DisplayName = "Just to know in save file if it is empty")
};

UENUM(BlueprintType)
enum class ActionType : uint8
{
	INTERACT_EXAMINE UMETA(DisplayName = "Examine the object"),
	INTERACT_OPENINVENTORY UMETA(DisplayName = "Open Inventory"),
	INTERACT_PICKUP UMETA(DisplayName = "Pickup Object"),
	INTERACT_ACTIVATE UMETA(DisplayName = "Activate or Use Object"),
	INTERACT_DIALOGUE UMETA(DisplayName = "Talk to npc")
};