// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"
#include "InventoryItem.h"
#include "ChestController.h"


// Sets default values for this component's properties
UChestController::UChestController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UChestController::BeginPlay()
{
	Super::BeginPlay();

	// Add what buttons this object has on its menu system...
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::OPEN]);
	Actions.Add(ActionType::INTERACT_OPENINVENTORY);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::PICKUP]);
	Actions.Add(ActionType::INTERACT_PICKUP);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::DIALOGUE]);
	Actions.Add(ActionType::INTERACT_DIALOGUE);

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

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}


// Called every frame
void UChestController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (SnappedToSlot)
	{
		GetOwner()->SetActorRotation(Rotation.Quaternion());
		GetOwner()->SetActorLocation(SlotLocation);
	}

	if (!filled)
	{
		if (toysfilled >= 6)
		{
			print("The chest is full it wants to rest now...");
			isOpen = false;
			locked = true;
			OPENID = -1;

			filled = true;
		}
	}
	

}

void UChestController::ActivateObject(AActor* Player)
{
	if (FollowWhenActivate)
	{
		PlayerToFollow = Player;
		FollowingPlayer = true;
		SnappedToSlot = false;
	}
}

void UChestController::ExamineObject(AActor* Player)
{
	print("It's a toy chest, something seems strange about it.");
}

void UChestController::OpenInventory(AActor* Player)
{
	if (!locked)
		isOpen = !isOpen;
}

void UChestController::PickupObject(AActor* Player)
{
	print("It's too heavy to pick up...");
}

void UChestController::ActivateDialogue(AActor* Player)
{
	// Max allowed dialogues is 6
	TArray<FString> DialogueOptions;
	DialogueOptions.Add("Hey... Go over to the x to the right");
	DialogueOptions.Add("Go over to the x to the back of the room...");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MainCharacter->SetDialogueOptions(DialogueOptions, this);
	MainCharacter->SetDialogueChoiceVisible();

	ShowAnimationBackwards = true;
	ShowRightClickMenu = false;
}

void UChestController::ItemInteract(int32 SlotNum)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());
	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);
	if (Item)
	{
		if (Item->INTERACT_ID == 666 && isOpen)
		{
			print("It likes toys!");
			toysfilled++;
			MainCharacter->DiscardItem(SlotNum);
			return;
		}
		else if (Item->INTERACT_ID == 666)
		{
			print("The chest is closed.");
		}

		if (Item->INTERACT_ID == OPENID)
		{
			locked = false;
			MainCharacter->DiscardItem(SlotNum);
			print("You hear a clicking noise!");
		}
		else if(Item->INTERACT_ID == OPENID)
		{
			print("You have a key, but it has the wrong ID!");
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString::FromInt(SlotNum));
	}
}

void UChestController::DialogueOptionPressed(UUserWidget* Caller, int optionindex)
{
	UE_LOG(LogTemp, Warning, TEXT("Option %s pressed."), *FString::FromInt(optionindex));
	Cast<AMainCharacter>(GameMode->GetMainCharacter())->SetDialogueChoiceInvisible();

	switch (optionindex)
	{
	case 0:
		Slot = 0;
		GoToSlot = true;
		FollowingPlayer = false;
		SnappedToSlot = false;
		break;
	case 1:
		Slot = 1;
		GoToSlot = true;
		FollowingPlayer = false;
		SnappedToSlot = false;
		break;
	default:
		print("This option is not possible!");
	}

	if (Hud)
		Hud->canPlayerClick = true;
}

AActor* UChestController::SlotToGoTo()
{
	//SnappedToSlot = false;
	switch (Slot)
	{
	case 0:
		return Slot1;
		break;
	case 1:
		return Slot2;
		break;
	default:
		return nullptr;
	}
}

void UChestController::SnapToSlot()
{
	AActor* SlotToSnapTo = SlotToGoTo();

	FVector SlotPos = SlotToSnapTo->GetActorLocation();
	
	FVector OurPos = GetOwner()->GetActorLocation();

	FRotator SlotRotation = SlotToSnapTo->GetActorRotation();

	SlotRotation.Yaw += 90;
	SlotLocation = FVector(SlotPos.X, SlotPos.Y, OurPos.Z);
	Rotation = SlotRotation;
	SnappedToSlot = true;
}