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

	// Legg til Meny-valg dynamisk s� vi slipper � lage en ny widget for hver eneste item
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

	// Hent peker til GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Hent peker til Hud klassen
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());


	// SubMenuWidgetClass definert i UInteractableComponent og initializert i konstruktoren der
	if (SubMenuWidgetClassTemplate)
	{
		// Lag widgeten i Hudden fordi vi ikke har lov til � gj�re det her!
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		// PointerGuard
		if (SubMenuWidget)
		{
			// Fortell HUD-Klassen at denne instansen har en meny!
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);

			// Add p� �verste lag p� viewporten
			SubMenuWidget->AddToViewport();
		}
	}

	// Fortell GameMode-Klassen at denne instansen er "Interactable", spilleren kan trykke p� og highlighte den
	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}


// Called every frame
void UChestController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Om kisten er snapped
	if (SnappedToSlot)
	{
		// Set posisjon og Rotasjon hver frame, p� grunn av AI-Blueprintet kan skape problemer av � til
		GetOwner()->SetActorRotation(Rotation.Quaternion());
		GetOwner()->SetActorLocation(SlotLocation);
	}

	// Hvis vi enda ikke har fyllt opp kisten med leker
	if (!filled)
	{
		// Sjekk om vi har nok leker
		if (toysfilled >= 6)
		{
			// Lukk kisten og gj�r det umulig � �pne den med en item
			print("The chest is full it wants to rest now...");
			isOpen = false;
			locked = true;
			OPENID = -1;

			// Den er full n�
			filled = true;
		}
	}
}

// Det som skjer n�r spilleren trykker med venstre mus-knapp p� et objekt i scenen
void UChestController::ActivateObject(AActor* Player)
{
	// F�lg etter spilleren hvis du har blitt aktivert
	if (FollowWhenActivate)
	{
		PlayerToFollow = Player;
		FollowingPlayer = true;
		SnappedToSlot = false;
	}
}

// N�r spilleren trykker "Examine" Meny valget
void UChestController::ExamineObject(AActor* Player)
{
	print("It's a toy chest, something seems strange about it.");
}

// N�r spilleren trykker "OpenInventory" Meny valget
void UChestController::OpenInventory(AActor* Player)
{
	// �pne kisten hvis den ikke er l�st
	if (!locked)
		isOpen = !isOpen;
}

// N�r spilleren trykker "PickupObject" Meny valget
void UChestController::PickupObject(AActor* Player)
{
	print("It's too heavy to pick up...");
}

// Det som skjer n�r spilleren trykker p� "Dialogue" meny valget
void UChestController::ActivateDialogue(AActor* Player)
{
	// Lag dialoger
	// Max allowed dialogues is 6
	TArray<FString> DialogueOptions;
	DialogueOptions.Add("Hey... Go over to the x to the right");
	DialogueOptions.Add("Go over to the x to the back of the room...");

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	// Fortell spilleren at den har dialoger
	MainCharacter->SetDialogueOptions(DialogueOptions, this);
	
	// Gj�r dialog-boksen synlig
	MainCharacter->SetDialogueChoiceVisible();

	// Hvis vi har en meny �pen, lukk den
	ShowAnimationBackwards = true;
	ShowRightClickMenu = false;
}

// Det som skjer dersom et objekt blir "sluppet" over dette objektet
void UChestController::ItemInteract(int32 SlotNum)
{
	// Hent hoved karakter pekeren ifra GameMode
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	// Hent itemet som er i den slotten vi slapp ifra, ( Se p� blueprintet InventorySlot )
	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);

	// Hvis pekeren ikke er NULL
	if (Item)
	{
		// Hvis det er en leke og kisten er �pen
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


		// Hvis det er en n�kkel du har sluppet over kisten
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

FBufferArchive UChestController::GetSaveData()
{
	FBufferArchive BinaryData;

	BinaryData << isOpen;
	BinaryData << FollowWhenActivate;
	BinaryData << FollowingPlayer;
	BinaryData << locked;
	BinaryData << GoToSlot;
	BinaryData << Slot;
	BinaryData << SnappedToSlot;
	BinaryData << Rotation;
	BinaryData << SlotLocation;
	BinaryData << filled;
	BinaryData << toysfilled;

	return BinaryData;
}

void UChestController::LoadSaveData(FBufferArchive & BinaryData)
{
	FMemoryReader Ar = FMemoryReader(BinaryData, true);

	// Load variables connected to this object...
	Ar << isOpen;
	Ar << FollowWhenActivate;
	Ar << FollowingPlayer;
	Ar << locked;
	Ar << GoToSlot;
	Ar << Slot;
	Ar << SnappedToSlot;
	Ar << Rotation;
	Ar << SlotLocation;
	Ar << filled;
	Ar << toysfilled;
}

// Dersom spilleren har valgt et dialogvalg
void UChestController::DialogueOptionPressed(UUserWidget* Caller, int optionindex)
{
	// Gj�r dialogboksen usynlig
	Cast<AMainCharacter>(GameMode->GetMainCharacter())->SetDialogueChoiceInvisible();

	// Switch for hvilket valg som ble trykt
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

	// Fortell Hudden at den du kan trykke p� ting i scenen igjen...
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