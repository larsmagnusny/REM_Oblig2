// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "PuzzleRiddle.h"
#include "MainCharacter.h"

UPuzzleRiddle::UPuzzleRiddle()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPuzzleRiddle::BeginPlay()
{
	Super::BeginPlay();

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

void UPuzzleRiddle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPuzzleRiddle::ActivateObject(AActor* Player)
{
	TArray<FString> Conversation;
	Conversation.Add("It's a book");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;

	MainCharacter->ShouldShowConversation = true;

	MainCharacter->SetDialogueChoiceVisible();
}

void UPuzzleRiddle::ExamineObject(AActor * Player)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);
	TArray<FString> Conversation;
	Conversation.Add("A book of riddles...");

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();

}
