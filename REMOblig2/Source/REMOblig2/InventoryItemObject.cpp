// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "InventoryItemObject.h"

AInventoryItemObject::AInventoryItemObject()
{
	// Instance the interactable component to this item...
	InventoryItemComponent = Cast<UInventoryItemComponent>(CreateDefaultSubobject<UInventoryItemComponent>(TEXT("InventoryItemComponent")));

	PrimaryActorTick.bCanEverTick = true;
}

AInventoryItemObject::~AInventoryItemObject()
{
	
}

void AInventoryItemObject::BeginPlay()
{
	Super::BeginPlay();

	// Hent Static Mesh Komponenten
	UStaticMeshComponent* Component = GetStaticMeshComponent();

	// Hent en peker til GameMode
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Sett verdiene i komponenten slik at dette objektet kan bevege seg og er simulert
	// Og kan kollidere med andre ting
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetSimulatePhysics(true);

	// Dersom Meshen ikke allerede er satt så henter vi en peker ifra MeshLoaderen
	if(!Mesh)
		Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemID);

	// Hvis et ikon ikke er satt så henter vi en peker ifra MeshLoaderen
	if(!InventoryIcon)
		InventoryIcon = GameMode->MeshesAndTextures->GetTextureByItemID(ItemID);

	// Sett kollisjonsprofilen
	Component->SetCollisionProfileName(FName("BlockAll"));
	Component->bGenerateOverlapEvents = true;
	Component->BodyInstance.bUseCCD = true;
	Component->SetNotifyRigidBodyCollision(true);

	// Sett Meshen
	Component->SetStaticMesh(Mesh);

	// Sett stor masse på objektet så spilleren ikke får den til å gå gjennom golvet når han går på den.
	Component->SetMassOverrideInKg(NAME_None, 100.f, true);

	// Dersom vi ikke har en item referanse så må vi lage en ny basert på den informasjonen vi har
	if (!InvItemRef)
		InvItemRef = new InventoryItem(ItemID, INTERACT_ID, Name, Mesh, InventoryIcon);

	// Set materials later
}
void AInventoryItemObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NeedDelete)
	{
		Destroy();
	}
}

void AInventoryItemObject::Init(InventoryItem* Item)
{
	if (Item)
	{
		InvItemRef = Item;
		ItemID = Item->ItemID;
		INTERACT_ID = Item->INTERACT_ID;
		Name = Item->Name;

		Mesh = Item->Mesh;
		InventoryIcon = Item->InventoryIcon;
	}
}