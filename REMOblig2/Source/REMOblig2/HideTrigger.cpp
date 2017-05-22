// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "HideTrigger.h"
#include "MainCharacter.h"

UHideTrigger::UHideTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHideTrigger::BeginPlay()
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

void UHideTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHideTrigger::ActivateObject(AActor* Player)
{
	TArray<FString> Conversation;
	Conversation.Add("Quickly hide by bressing the spacebar!");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MainCharacter->Conversation = Conversation;

	MainCharacter->ShouldShowConversation = true;

	MainCharacter->SetDialogueChoiceVisible();
}



