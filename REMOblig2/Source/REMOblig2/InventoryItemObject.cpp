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

	// Dersom Meshen ikke allerede er satt s� henter vi en peker ifra MeshLoaderen
	if(!Mesh)
		Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemID);

	// Hvis et ikon ikke er satt s� henter vi en peker ifra MeshLoaderen
	if(!InventoryIcon)
		InventoryIcon = GameMode->MeshesAndTextures->GetTextureByItemID(ItemID);

	// Sett kollisjonsprofilen
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Component->bGenerateOverlapEvents = true;
	Component->BodyInstance.bUseCCD = true;
	Component->SetNotifyRigidBodyCollision(true);

	// Sett Meshen
	Component->SetStaticMesh(Mesh);

	// Sett Materialene
	TArray<UMaterial*> Mats = GameMode->MeshesAndTextures->GetMaterialsByItemID(ItemID);
	for (int i = 0; i < Mats.Num(); i++)
	{
		Component->SetMaterial(i, Mats[i]);
	}

	// Sett stor masse p� objektet s� spilleren ikke f�r den til � g� gjennom golvet n�r han g�r p� den.
	Component->SetMassOverrideInKg(NAME_None, 100.f, true);

	// Dersom vi ikke har en item referanse s� m� vi lage en ny basert p� den informasjonen vi har
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