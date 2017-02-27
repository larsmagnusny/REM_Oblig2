// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "InventoryItemObject.h"

AInventoryItemObject::AInventoryItemObject()
{

}

AInventoryItemObject::~AInventoryItemObject()
{

}

void AInventoryItemObject::BeginPlay()
{
	Super::BeginPlay();
}
void AInventoryItemObject::Tick()
{
	Super::Tick();
}

void AInventoryItemObject::Init(InventoryItem* Item)
{
	InvItemRef = Item;
}