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

	// Lets Load a skeletalMesh to attach to our socket... Because science, also need to load in a animation blueprint for it...
	ConstructorHelpers::FObjectFinder<USkeletalMesh> LockLoader(TEXT("SkeletalMesh'/Game/Meshes/LockAndKey/LockAndKey.LockAndKey'"));

	ConstructorHelpers::FObjectFinder<UClass> LockAnimLoader(TEXT("Class'/Game/Meshes/LockAndKey/LockAndKey_AnimBP.LockAndKey_AnimBP_C'"));

	if (LockLoader.Succeeded() && LockAnimLoader.Succeeded())
	{
		LockSkeletalMesh = LockLoader.Object;
		AnimBPClass = LockAnimLoader.Object;
	}

	LockHolder = CreateDefaultSubobject<USkeletalMeshComponent>(FName("LockHolderComponent"), true);

	for (int i = 0; i < 6; i++)
	{
		FString ItemName = "ItemHolder" + FString::FromInt(i);
		ChestItemSlots.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*ItemName), true));
	}

	ChestSlotDelegation.Add(ItemIDs::ITEM_SPINNER);
	ChestSlotDelegation.Add(ItemIDs::ITEM_YELLOW_LEGO);
	ChestSlotDelegation.Add(ItemIDs::ITEM_BLUE_LEGO);
	ChestSlotDelegation.Add(ItemIDs::ITEM_PURPLE_LEGO);
	ChestSlotDelegation.Add(ItemIDs::ITEM_ORANGE_LEGO);
	ChestSlotDelegation.Add(ItemIDs::ITEM_SOUNDTOY);
}


// Called when the game starts
void UChestController::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);

	USkeletalMeshComponent* ParentSkeletalMesh = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (ParentSkeletalMesh)
	{
		LockHolder->AttachToComponent(ParentSkeletalMesh, Rules, FName("LockHolder"));
		LockHolder->RegisterComponent();
	}

	// Snap Slot Components
	ChestItemSlots[0]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item1"));
	ChestItemSlots[1]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item2"));
	ChestItemSlots[2]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item3"));
	ChestItemSlots[3]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item4"));
	ChestItemSlots[4]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item5"));
	ChestItemSlots[5]->AttachToComponent(ParentSkeletalMesh, Rules, FName("Item6"));

	for (int i = 0; i < 6; i++)
	{
		ChestItemSlots[i]->RegisterComponent();
	}


	// Set Mesh to LockHolder...
	if (AnimBPClass)
		LockHolder->SetAnimInstanceClass(AnimBPClass);

	if (LockSkeletalMesh)
		LockHolder->SetSkeletalMesh(LockSkeletalMesh);

	if (locked)
	{

	}

	// Legg til Meny-valg dynamisk så vi slipper å lage en ny widget for hver eneste item
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::OPEN]);
	Actions.Add(ActionType::INTERACT_OPENINVENTORY);

	//ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::PICKUP]);
	//Actions.Add(ActionType::INTERACT_PICKUP);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	//ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::DIALOGUE]);
	//Actions.Add(ActionType::INTERACT_DIALOGUE);

	// Hent peker til GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Hent peker til Hud klassen
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());


	// SubMenuWidgetClass definert i UInteractableComponent og initializert i konstruktoren der
	if (SubMenuWidgetClassTemplate)
	{
		// Lag widgeten i Hudden fordi vi ikke har lov til å gjøre det her!
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		// PointerGuard
		if (SubMenuWidget)
		{
			// Fortell HUD-Klassen at denne instansen har en meny!
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);

			// Add på øverste lag på viewporten
			SubMenuWidget->AddToViewport(11);
		}
	}

	// Fortell GameMode-Klassen at denne instansen er "Interactable", spilleren kan trykke på og highlighte den
	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}


// Called every frame
void UChestController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Om kisten er snapped
	if (SnappedToSlot)
	{
		// Set posisjon og Rotasjon hver frame, på grunn av AI-Blueprintet kan skape problemer av å til
		GetOwner()->SetActorRotation(Rotation.Quaternion());
		GetOwner()->SetActorLocation(SlotLocation);
	}

	// Hvis vi enda ikke har fyllt opp kisten med leker
	if (!filled)
	{
		// Sjekk om vi har nok leker
		if (toysfilled >= 6)
		{
			// Lukk kisten og gjør det umulig å åpne den med en item
			isOpen = false;
			locked = true;
			OPENID = -1;

			// Den er full nå
			filled = true;
		}
	}

	if (FollowingPlayer)
	{
		float DistToSlot = (Slot1->GetActorLocation() - GetOwner()->GetActorLocation()).Size();

		if (DistToSlot < 200)
		{
			Slot = 0;
			GoToSlot = true;
			FollowingPlayer = false;
			SnappedToSlot = false;
		}
	}
}

// Det som skjer når spilleren trykker med venstre mus-knapp på et objekt i scenen
void UChestController::ActivateObject(AActor* Player)
{
	// Følg etter spilleren hvis du har blitt aktivert
	ExamineObject(Player);
}

// Når spilleren trykker "Examine" Meny valget
void UChestController::ExamineObject(AActor* Player)
{
	if (FollowingPlayer)
	{
		TArray<FString> Conversation;

		Conversation.Add("The chest is following me. I can't open it. Seems to be locked.");

		Cast<AMainCharacter>(Player)->Conversation = Conversation;
		Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
		Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
	}
	else if (SnappedToSlot)
	{
		TArray<FString> Conversation;

		Conversation.Add("The chest is sitting on some kind of mechanism...");

		Cast<AMainCharacter>(Player)->Conversation = Conversation;
		Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
		Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
	}
	else {
		TArray<FString> Conversation;

		Conversation.Add("You can see toes sticking up from underneath the chest. That is very strange.");

		Cast<AMainCharacter>(Player)->Conversation = Conversation;
		Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
		Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
	}
}

// Når spilleren trykker "OpenInventory" Meny valget
void UChestController::OpenInventory(AActor* Player)
{
	// Åpne kisten hvis den ikke er låst
	if (!locked)
		isOpen = !isOpen;
	else
	{
		TArray<FString> Conversation;

		Conversation.Add("I can't open it. The chest is locked...");

		Cast<AMainCharacter>(Player)->Conversation = Conversation;
		Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
		Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
	}
}

// Når spilleren trykker "PickupObject" Meny valget
void UChestController::PickupObject(AActor* Player)
{
	print("It's too heavy to pick up...");
}

// Det som skjer når spilleren trykker på "Dialogue" meny valget
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
	
	// Gjør dialog-boksen synlig
	MainCharacter->SetDialogueChoiceVisible();

	// Hvis vi har en meny åpen, lukk den
	ShowAnimationBackwards = true;
	ShowRightClickMenu = false;
}

// Det som skjer dersom et objekt blir "sluppet" over dette objektet
void UChestController::ItemInteract(int32 SlotNum)
{
	// Hent hoved karakter pekeren ifra GameMode
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	// Hent itemet som er i den slotten vi slapp ifra, ( Se på blueprintet InventorySlot )
	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);


	TArray<FString> Conversation;

	// Hvis pekeren ikke er NULL
	if (Item)
	{
		if (Item->INTERACT_ID == 10101)
		{
			RunStandupAnimation = true;
			PlayerToFollow = GameMode->GetMainCharacter();
			FollowingPlayer = true;
			MainCharacter->DiscardItem(SlotNum);

			TArray<FString> Conversation;

			Conversation.Add("You tickle the toes of the chest. It suddenly stands up. It seems set on following you.");

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		}

		// Hvis det er en leke og kisten er åpen
		if (Item->INTERACT_ID == 666 && isOpen)
		{
			toysfilled++;
			ShowToy(Item->ItemID);
			MainCharacter->DiscardItem(SlotNum);
			return;
		}
		else if (Item->INTERACT_ID == 666)
		{
			Conversation.Add("I need to open the chest before i can put something in there.");
			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		}


		// Hvis det er en nøkkel du har sluppet over kisten
		if (Item->INTERACT_ID == OPENID)
		{
			locked = false;
			MainCharacter->DiscardItem(SlotNum);
			Conversation.Add("You hear the chest open with a \"click\" of the lock.");
			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		}
		else if(Item->INTERACT_ID != OPENID && Item->ItemID == ItemIDs::ITEM_KEY)
		{
			Conversation.Add("The key you have doesn't fit the lock?!");
			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString::FromInt(SlotNum));
	}
}

FVector UChestController::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector()*50.f;
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

void UChestController::LoadSaveData(FMemoryReader &Ar)
{
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

void UChestController::ShowToy(ItemIDs ID)
{
	int index = -1;

	for (int i = 0; i < 6; i++)
	{
		if (ChestSlotDelegation[i] == ID)
			index = i;
	}

	if (index != -1)
	{
		UStaticMesh* Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ID);
		TArray<UMaterial*> Materials = GameMode->MeshesAndTextures->GetMaterialsByItemID(ID);
		ChestItemSlots[index]->SetStaticMesh(Mesh);

		for (int i = 0; i < Materials.Num(); i++)
		{
			ChestItemSlots[index]->SetMaterial(i, Materials[i]);
		}
	}
}

// Dersom spilleren har valgt et dialogvalg
void UChestController::DialogueOptionPressed(UUserWidget* Caller, int optionindex)
{
	// Gjør dialogboksen usynlig
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
	RunStitdownAnimation = true;
}