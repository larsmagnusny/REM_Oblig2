// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"
#include "InventoryItemObject.h"
#include "InventoryItemComponent.h"

UInventoryItemComponent::UInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryItemComponent::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Began!"));

	// Add what buttons this object has on its menu system...
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::PICKUP]);
	Actions.Add(ActionType::INTERACT_PICKUP);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

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

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}
void UInventoryItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldDie)
	{
		ShouldDie = false;
		GetOwner()->Destroy();
	}
}

void UInventoryItemComponent::ActivateObject(AActor* Player)
{
	PickupObject(Player);
}

void UInventoryItemComponent::ExamineObject(AActor* Player)
{
	TArray<FString> Conversation;

	Conversation.Add("This item has no description.");

	Cast<AMainCharacter>(Player)->Conversation = Conversation;
	Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
	Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
}

void UInventoryItemComponent::PickupObject(AActor* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Should tell the GameMode to remove me as interactable and delete me then add me to the player inventory!"));
	// Hent en referanse til denne itemet ifra parenten som er et objekt vi spawner inn i verden
	InventoryItem* ThisItem = Cast<AInventoryItemObject>(GetOwner())->InvItemRef;

	if (ThisItem)
	{
		// Fortell HovedKarakteren at han skal legge til ThisItem til inventoryen hans
		AMainCharacter* Character = Cast<AMainCharacter>(GameMode->GetMainCharacter());

		bool SuccessfullyAdded = Character->AddItemToInventory(ThisItem);
		//Cast<AMainCharacter>(GameMode->GetMainCharacter())->AddItemToInventory(ThisItem);


		if (SuccessfullyAdded)
		{
			// Hvis jeg ikke fjerner denne så kan spillet krashe
			if(GameMode->IsInteractble(GetOwner()))
				GameMode->RemoveInteractableObject(GetOwner());

			if (Hud)
			{
				Hud->RemoveInteractionWidget(this);
				UE_LOG(LogTemp, Warning, TEXT("Should show animation backwards..."));
			}

			ShouldDie = true;
		}
	}
}

FVector UInventoryItemComponent::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation();
}
