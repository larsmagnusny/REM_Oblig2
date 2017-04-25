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

	this->OnActorHit.AddDynamic(this, &AInventoryItemObject::OnHit);

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

	if (!CanPlaySound)
	{
		if (HitTimer < MaxHitTime)
		{
			HitTimer += DeltaTime;
		}
		else {
			HitTimer = 0.f;
			CanPlaySound = true;
		}
	}

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

void AInventoryItemObject::OnHit(AActor* OtherActor, AActor* MyActor, FVector Normal, const FHitResult& Hit)
{
	if (!CanPlaySound)
		return;

	CanPlaySound = false;

	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Play a random sound sound...
	TArray<USoundWave*> SoundsToPlay;

	SoundsToPlay.Add(GameMode->SoundLoaderInstance->Sounds[(uint8)Sounds::SOUND_OBJECTHIT1]);
	SoundsToPlay.Add(GameMode->SoundLoaderInstance->Sounds[(uint8)Sounds::SOUND_OBJECTHIT2]);
	SoundsToPlay.Add(GameMode->SoundLoaderInstance->Sounds[(uint8)Sounds::SOUND_OBJECTHIT3]);
	SoundsToPlay.Add(GameMode->SoundLoaderInstance->Sounds[(uint8)Sounds::SOUND_OBJECTHIT4]);
	SoundsToPlay.Add(GameMode->SoundLoaderInstance->Sounds[(uint8)Sounds::SOUND_OBJECTHIT5]);

	int RandMax = SoundsToPlay.Num() - 1;

	int RandomSound = FMath::RandRange(0, RandMax);

	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundsToPlay[RandomSound], GetActorLocation(), GameMode->GameInstance->SFXVolume*10);
}
