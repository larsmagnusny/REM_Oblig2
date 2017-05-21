// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "BoxTest.h"
#include"MainCharacter.h"

UBoxTest::UBoxTest()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoxTest::BeginPlay()
{
	Super::BeginPlay();

	//Legg til menyvalgene
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

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
			SubMenuWidget->AddToViewport(11);
		}
	}
	GameMode->AddInteractableObject(GetOwner(), this);
}

void UBoxTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBoxTest::ActivateObject(AActor * Player)
{
	TArray<FString> Conversation;
	Conversation.Add("This is a box");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;

	MainCharacter->ShouldShowConversation = true;

	MainCharacter->SetDialogueChoiceVisible();
}

void UBoxTest::ItemInteract(int32 SlotNum)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());
	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);

	if (Item)
	{
		if (Item->INTERACT_ID == 666)
		{
			TArray<FString> Conversation;
			Conversation.Add("You try to use the key on the box, it bounces off...");

			MainCharacter->Conversation = Conversation;

			MainCharacter->ShouldShowConversation = true;

			MainCharacter->SetDialogueChoiceVisible();
		}
		else
		{
			TArray<FString> Conversation;
			Conversation.Add("What are you doing?");

			MainCharacter->Conversation = Conversation;

			MainCharacter->ShouldShowConversation = true;

			MainCharacter->SetDialogueChoiceVisible();
		}
	}
}

void UBoxTest::ExamineObject(AActor * Player)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);
	TArray<FString> Conversation;
	Conversation.Add("The box is breathing");

	MainCharacter->Conversation = Conversation;

	MainCharacter->ShouldShowConversation = true;

	MainCharacter->SetDialogueChoiceVisible();
}
