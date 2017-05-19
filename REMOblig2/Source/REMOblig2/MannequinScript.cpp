// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MannequinScript.h"
#include "MainCharacter.h"

UMannequinScript::UMannequinScript()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMannequinScript::BeginPlay()
{
	Super::BeginPlay();

	// Legg til Meny-valg dynamisk så vi slipper å lage en ny widget for hver eneste item
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::DIALOGUE]);
	Actions.Add(ActionType::INTERACT_DIALOGUE);

	// Hent en peker til GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);

			SubMenuWidget->AddToViewport(11);
		}
	}

	GameMode->AddInteractableObject(GetOwner(), this);
}

void UMannequinScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

void UMannequinScript::ActivateObject(AActor * Player)
{
	TArray<FString> Conversation;

	if (!HasBeenExamined)
	{
		ExamineObject(Player);
		
		return;
	}
	if (ItemHasDropped)
	{
		ExamineObject(Player);
		return;
	}
	if (ItemDropsWhenItemInteracts)
		Conversation.Add("You try to drag off the arm. You're not strong enough.");
	else
		Conversation.Add("I don't want to touch that thing.");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();
}

void UMannequinScript::ExamineObject(AActor * Player)
{
	HasBeenExamined = true;
	TArray<FString> Conversation;
	if (ItemHasDropped)
		Conversation.Add("It's standing there with one hand staring judgementally at you.");
	else if (ItemDropsWhenItemInteracts)
		Conversation.Add("The mannequin has an arm loose.");
	else
		Conversation.Add("A creepy mannequin, it stares into your soul.");

	AMainCharacter* MainCharacter = Cast <AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();
}

void UMannequinScript::ActivateDialogue(AActor * Player)
{
	TArray<FString> Conversation;


	Conversation.Add("You: Hello!");
	Conversation.Add("Mannequin: *stares at you*");

	AMainCharacter* MainCharacter = Cast <AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();
}

void UMannequinScript::ItemInteract(int32 SlotNum)
{
	if (ItemHasDropped)
		return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);

	TArray<FString> Conversation;

	if (Item)
	{
		if (Item->INTERACT_ID == InteractID)
		{
			Conversation.Add("The arm falls to the ground and you see a toy fall out of it.");

			// Spawn a toy...
			InventoryItem* SpawnItem = new InventoryItem(ItemToDrop, 666, "Toy", GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemToDrop), GameMode->MeshesAndTextures->GetTextureByItemID(ItemToDrop));

			FVector Position = GetActivatePosition(MainCharacter) + GetOwner()->GetActorRightVector()*50.f;

			GameMode->PutObjectInWorld(SpawnItem, Position, FVector(0, 0, 0), FVector(1, 1, 1));

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();

			ItemHasDropped = true;
		}
	}
}

FVector UMannequinScript::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation();
}

FBufferArchive UMannequinScript::GetSaveData()
{
	FBufferArchive BinaryData;
	BinaryData << ItemHasDropped;
	BinaryData << HasBeenExamined;
	return BinaryData;
}

void UMannequinScript::LoadSaveData(FMemoryReader & Ar)
{
	Ar << ItemHasDropped;
	Ar << HasBeenExamined;
}
