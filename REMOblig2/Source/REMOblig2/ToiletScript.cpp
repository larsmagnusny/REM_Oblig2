// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "ToiletScript.h"
#include "MainCharacter.h"

UToiletScript::UToiletScript()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UToiletScript::BeginPlay()
{
	Super::BeginPlay();

	// Legg til Meny-valg dynamisk så vi slipper å lage en ny widget for hver eneste item
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

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

void UToiletScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

void UToiletScript::ActivateObject(AActor * Player)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);
	if (!HasBeenExamined)
	{
		ExamineObject(Player);
		
	}
	else {
		if (!ItemAddedToInventory)
		{
			TArray<FString> Conversation;

			Conversation.Add("Oh... You find a pair of pliers in the toiled reservoir. That's unexpected. *Pliers Added To Inventory*");

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();

			InventoryItem* Pliers = new InventoryItem(ItemToAddToInventoryOnActivate, 911375, "Pliers", GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemToAddToInventoryOnActivate), GameMode->MeshesAndTextures->GetTextureByItemID(ItemToAddToInventoryOnActivate));

			MainCharacter->AddItemToInventory(Pliers);
			ItemAddedToInventory = true;
		}
	}
}

void UToiletScript::ExamineObject(AActor * Player)
{
	HasBeenExamined = true;
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	TArray<FString> Conversation;

	Conversation.Add("It's a toilet, what do you expect to find here....");

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();

}

FBufferArchive UToiletScript::GetSaveData()
{
	FBufferArchive BinaryData;
	BinaryData << ItemAddedToInventory;
	BinaryData << HasBeenExamined;
	return BinaryData;
}

void UToiletScript::LoadSaveData(FMemoryReader & Ar)
{
	Ar << ItemAddedToInventory;
	Ar << HasBeenExamined;
}
