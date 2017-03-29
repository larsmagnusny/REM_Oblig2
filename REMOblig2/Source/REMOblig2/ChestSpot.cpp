// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InventoryItem.h"
#include "MainCharacter.h"
#include "LockedDoor.h"
#include "ChestSpot.h"

UChestSpot::UChestSpot()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UChestSpot::BeginPlay()
{
	Super::BeginPlay();

	// Legg til meny-valg for "Right-Click" Meny
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// SubMenuWidgetClass definert i UInteractableComponent
	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport();
		}
	}

	// Fortell gamemode at denne er interactable

	GameMode->AddInteractableObject(GetOwner(), this);
}

void UChestSpot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Dersom vi har en peker til Døren og kisten så vet vi at denne kan løse puzzelet dersom kisten er full
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
						
						TArray<FString> Conversation;
						Conversation.Add("You hear the door open.");

						Cast<AMainCharacter>(GameMode->GetMainCharacter())->Conversation = Conversation;
						Cast<AMainCharacter>(GameMode->GetMainCharacter())->ShouldShowConversation = true;
						Cast<AMainCharacter>(GameMode->GetMainCharacter())->SetDialogueChoiceVisible();

						PuzzleSolved = true;
					}
				}
			}
		}
	}	// Kan droppe en item.
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

						// Spawn ett objekt
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
	TArray<FString> Conversation;
	Conversation.Add("There's an x marking the spot. I wonder if i place something here something will happen?");

	Cast<AMainCharacter>(Player)->Conversation = Conversation;
	Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
	Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
}

FBufferArchive UChestSpot::GetSaveData()
{
	FBufferArchive BinaryData;

	BinaryData << ItemDropped;
	BinaryData << PuzzleSolved;

	return BinaryData;
}

void UChestSpot::LoadSaveData(FMemoryReader &Ar)
{
	Ar << ItemDropped;
	Ar << PuzzleSolved;
}

FVector UChestSpot::GetActivatePosition(AActor* Player)
{
	return GetOwner()->GetActorLocation();
}