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
	// Add what buttons this object has on its menu system...
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::PICKUP]);
	Actions.Add(ActionType::INTERACT_PICKUP);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (SubMenuWidgetClassTemplate && Hud)
	{

		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport(11);
		}
	}

	PickupScale = GetOwner()->GetActorScale3D();

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
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


	AInventoryItemObject* Owner = Cast<AInventoryItemObject>(GetOwner());

	if (Owner->ItemID == ItemIDs::ITEM_RADIO)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(Player);

		if (Character)
			Character->RadioVisible = true;

		// Hvis jeg ikke fjerner denne s� kan spillet krashe
		if (GameMode->IsInteractble(GetOwner()))
			GameMode->RemoveInteractableObject(GetOwner());

		if (Hud)
		{
			Hud->RemoveInteractionWidget(this);
		}

		// Show a illustration of the item...
		ShouldDie = true;
		Hud->HintSnapToActor = nullptr;
	}
	else
	{
		PickupObject(Player);
	}
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
	// Hent en referanse til denne itemet ifra parenten som er et objekt vi spawner inn i verden
	InventoryItem* ThisItem = Cast<AInventoryItemObject>(GetOwner())->InvItemRef;

	if (ThisItem)
	{
		// Fortell HovedKarakteren at han skal legge til ThisItem til inventoryen hans
		AMainCharacter* Character = Cast<AMainCharacter>(GameMode->GetMainCharacter());

		bool SuccessfullyAdded = Character->AddItemToInventory(ThisItem);

		if (SuccessfullyAdded)
		{
			// Hvis jeg ikke fjerner denne s� kan spillet krashe
			if(GameMode->IsInteractble(GetOwner()))
				GameMode->RemoveInteractableObject(GetOwner());

			if (Hud)
			{
				Hud->RemoveInteractionWidget(this);
			}

			// Show a illustration of the item...
			ShouldDie = true;
			Hud->HintSnapToActor = nullptr;
		}
		else {
			TArray<FString> Conversation;

			Conversation.Add("My inventory is full, i need to drop something to pick this up...");

			Cast<AMainCharacter>(Player)->Conversation = Conversation;
			Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
			Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();

			AInventoryItemObject* Owner = Cast<AInventoryItemObject>(GetOwner());
			Owner->GetStaticMeshComponent()->SetVisibility(true, true);
		}
	}
}

FVector UInventoryItemComponent::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation();
}
