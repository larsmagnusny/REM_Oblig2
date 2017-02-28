// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InventoryItem.h"
#include "LockedDoor.h"
#include "ChestSpot.h"

UChestSpot::UChestSpot()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Add what buttons this object has on its menu system...
	
}

void UChestSpot::BeginPlay()
{
	Super::BeginPlay();

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport();
		}
	}

	GameMode->AddInteractableObject(GetOwner(), this);
}

void UChestSpot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Door && Chest)
	{
		if (!ChestScriptInstance)
			ChestScriptInstance = Cast<UChestController>(Chest->GetComponentByClass(UChestController::StaticClass()));

		if (ChestScriptInstance)
		{
			if (ChestScriptInstance->SnappedToSlot)
			{
				if (ChestScriptInstance->SlotToGoTo() == GetOwner())
				{
					if (ChestScriptInstance->filled && !PuzzleSolved)
					{
						ULockedDoor* DoorInteractor = Cast<ULockedDoor>(GameMode->GetInteractor(Door));
						DoorInteractor->SetPuzzleSolved();
						print("You hear the door open!");
						PuzzleSolved = true;
					}
				}
			}
		}
	}
	else if (Chest)
	{
		if(!ChestScriptInstance)
			ChestScriptInstance = Cast<UChestController>(Chest->GetComponentByClass(UChestController::StaticClass()));

		if (ChestScriptInstance)
		{
			if (ChestScriptInstance->SnappedToSlot)
			{
				if (ChestScriptInstance->SlotToGoTo() == GetOwner())
				{
					if (CanDropItem && !ItemDropped)
					{
						UStaticMesh* Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemToDrop);
						UTexture2D* Texture = GameMode->MeshesAndTextures->GetTextureByItemID(ItemToDrop);
						InventoryItem* Item = new InventoryItem(ItemToDrop, OPEN_ID, "Key", Mesh, Texture);

						GameMode->PutObjectInWorld(Item, GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector()*200.f + GetOwner()->GetActorForwardVector()*100.f, FVector(0, 0, 0), FVector(1, 1, 1));
						ItemDropped = true;
					}
				}
			}
		}
	}
}

void UChestSpot::ActivateObject(AActor* Player)
{
	ExamineObject(Player);
}

void UChestSpot::ExamineObject(AActor* Player)
{
	print("There's an x marking the spot. I wonder if i place something here something will happen?");
}

FVector UChestSpot::GetActivatePosition(AActor* Player)
{
	return GetOwner()->GetActorLocation();
}