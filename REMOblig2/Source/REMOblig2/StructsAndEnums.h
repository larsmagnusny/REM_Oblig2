// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

/**
 * 
 */

UENUM(BlueprintType)
enum class ActionType : uint8
{
	INTERACT_EXAMINE UMETA(DisplayName = "Examine the object"),
	INTERACT_OPENINVENTORY UMETA(DisplayName = "Open Inventory"),
	INTERACT_PICKUP UMETA(DisplayName = "Pickup Object"),
	INTERACT_ACTIVATE UMETA(DisplayName = "Activate or Use Object"),
	INTERACT_DIALOGUE UMETA(DisplayName = "Talk to npc")
};

class REMOBLIG2_API StructsAndEnums
{
public:
	StructsAndEnums();
	~StructsAndEnums();
};
