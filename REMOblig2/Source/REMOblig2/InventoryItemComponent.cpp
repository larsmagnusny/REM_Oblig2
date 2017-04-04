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

	UE_LOG(LogTemp, Warning, TEXT("Buttons Added!"));

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	UE_LOG(LogTemp, Warning, TEXT("References gotten..."));

	if (SubMenuWidgetClassTemplate && Hud)
	{

		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);
		UE_LOG(LogTemp, Warning, TEXT("Widget Created"));

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			UE_LOG(LogTemp, Warning, TEXT("Added Interaction Widget"));
			SubMenuWidget->AddToViewport(11);
			UE_LOG(LogTemp, Warning, TEXT("Added to viewport"));
		}
	}
	

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);

	UE_LOG(LogTemp, Warning, TEXT("Set Interactable!"));
}
void UInventoryItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldDie)
	{
		ShouldDie = false;
		Cast<AInventoryItemObject>(GetOwner())->NeedDelete = true;
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

	UE_LOG(LogTemp, Warning, TEXT("ItemRef Got"));

	if (ThisItem)
	{
		// Fortell HovedKarakteren at han skal legge til ThisItem til inventoryen hans
		AMainCharacter* Character = Cast<AMainCharacter>(GameMode->GetMainCharacter());
		UE_LOG(LogTemp, Warning, TEXT("Character Found"));

		bool SuccessfullyAdded = Character->AddItemToInventory(ThisItem);
		UE_LOG(LogTemp, Warning, TEXT("Added To Inventory"));


		if (SuccessfullyAdded)
		{
			// Hvis jeg ikke fjerner denne så kan spillet krashe
			UE_LOG(LogTemp, Warning, TEXT("Checking Interactable"));
			if(GameMode->IsInteractble(GetOwner()))
				GameMode->RemoveInteractableObject(GetOwner());

			UE_LOG(LogTemp, Warning, TEXT("Removed if found"));

			if (Hud)
			{
				UE_LOG(LogTemp, Warning, TEXT("Trying to remove interactionWidget"));
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
