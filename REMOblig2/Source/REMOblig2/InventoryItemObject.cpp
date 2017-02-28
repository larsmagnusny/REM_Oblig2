// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "InventoryItemObject.h"

AInventoryItemObject::AInventoryItemObject()
{
	// Instance the interactable component to this item...
	InventoryItemComponent = Cast<UInventoryItemComponent>(CreateDefaultSubobject<UInventoryItemComponent>(TEXT("InventoryItemComponent")));

	
}

AInventoryItemObject::~AInventoryItemObject()
{
	
}

void AInventoryItemObject::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* Component = GetStaticMeshComponent();

	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	Component->SetMobility(EComponentMobility::Movable);
	Component->SetSimulatePhysics(true);
	Component->bGenerateOverlapEvents = true;

	if(!Mesh)
		Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemID);
	if(!InventoryIcon)
		InventoryIcon = GameMode->MeshesAndTextures->GetTextureByItemID(ItemID);

	Component->SetCollisionProfileName(FName("BlockAll"));
	Component->SetStaticMesh(Mesh);
	Component->SetMassOverrideInKg(NAME_None, 100.f, true);

	if (!InvItemRef)
		InvItemRef = new InventoryItem(ItemID, INTERACT_ID, Name, Mesh, InventoryIcon);

	// Set materials later
}
void AInventoryItemObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventoryItemObject::Init(InventoryItem* Item)
{
	InvItemRef = Item;
	ItemID = Item->ItemID;
	INTERACT_ID = Item->INTERACT_ID;
	Name = Item->Name;

	Mesh = Item->Mesh;
	InventoryIcon = Item->InventoryIcon;
}