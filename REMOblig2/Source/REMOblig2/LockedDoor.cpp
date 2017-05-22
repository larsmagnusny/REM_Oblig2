// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LockedDoor.h"
#include "REM_GameMode.h"
#include "MainCharacter.h"
#include "REM_Hud.h"

ULockedDoor::ULockedDoor()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void ULockedDoor::BeginPlay()
{
	Super::BeginPlay();

	// Meny valg som spilleren kan trykke på
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	// Hent peker til HUD og GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	AREM_Hud* Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport(11);
		}
	}

	// Så GameMode er klar over at vi kan interactes med
	GameMode->AddInteractableObject(GetOwner(), this);

	// Den rotasjonen døren går tilbake til når den lukkes
	InitialRotation = GetOwner()->GetActorRotation();

	// Used to check which side of the door the player is, so it doesn't slam him in the face when he tries to open it!
	if (OpenDirection)
		OpenDir = -GetOwner()->GetActorRightVector();
	else
		OpenDir = GetOwner()->GetActorRightVector();
}

void ULockedDoor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Brukes i puzzle
	if (DoorOpenCondition == OpenCondition::OPEN_COMPLETE_PUZZLE && PuzzleSolved)
	{
		Open = true;
	}

	if (Open)
	{
		if (CurrentRotation < MaxOpenAngle)
		{
			CurrentRotation += OpenSpeed*DeltaTime;
		}
		else
		{
			CurrentRotation = MaxOpenAngle;
		}

		FRotator NewRotation = InitialRotation;

		if (OpenDirection)
			NewRotation.Yaw += CurrentRotation;
		else
			NewRotation.Yaw -= CurrentRotation;

		GetOwner()->SetActorRotation(NewRotation.Quaternion(), ETeleportType::None);
	}
	else {
		if (CurrentRotation > 0)
		{
			CurrentRotation -= OpenSpeed*DeltaTime;
		}
		else {
			CurrentRotation = 0.f;
		}

		FRotator NewRotation = InitialRotation;

		if (OpenDirection)
			NewRotation.Yaw += CurrentRotation;
		else
			NewRotation.Yaw -= CurrentRotation;

		GetOwner()->SetActorRotation(NewRotation.Quaternion(), ETeleportType::None);
	}
}

void ULockedDoor::ActivateObject(AActor* Player)
{
	// Dersom døren kan åpnes normalt
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		if (Open)
			CloseDoor();
		else
			OpenDoor();
	}
	else {
		// Ikke åpne den hvis den er låst på en eller annen måte.
		ExamineObject(Player);
	}
}

void ULockedDoor::ExamineObject(AActor* Player)
{
	TArray<FString> Conversation;
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		Conversation.Add("The door is unlocked...");
	}
	if (DoorOpenCondition == OpenCondition::OPEN_KEY)
	{
		Conversation.Add("A key is required to open this door.");
	}
	if (DoorOpenCondition == OpenCondition::OPEN_COMPLETE_PUZZLE)
	{
		Conversation.Add("The door wont budge, maybe i still need to do something in this room.");
	}
	if (DoorOpenCondition == OpenCondition::OPEN_TRIGGER)
	{
		Conversation.Add("There are wires coming out of the edge of the door seemingly connecting to a mechanism.");
	}

	Cast<AMainCharacter>(Player)->Conversation = Conversation;
	Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
	Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
}

void ULockedDoor::ItemInteract(int32 SlotNum)
{
	// Hent hoved karakter pekeren ifra GameMode
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	// Hent itemet som er i den slotten vi slapp ifra, ( Se på blueprintet InventorySlot )
	InventoryItem* Item = MainCharacter->GetItemBySlot(SlotNum);


	TArray<FString> Conversation;

	// Hvis pekeren ikke er NULL
	if (Item)
	{
		if (Item->INTERACT_ID == OpenID)
		{
			Conversation.Add("The door unlocks.");
			DoorOpenCondition = OpenCondition::OPEN_NORMAL;
			MainCharacter->DiscardItem(SlotNum);

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		}
		else if (Item->INTERACT_ID != OpenID && Item->ItemID == ItemIDs::ITEM_KEY)
		{
			Conversation.Add("The key does not fit the lock");

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();
		} 
		else
		{
			Super::ItemInteract(SlotNum);
		}
	}
}

void ULockedDoor::OpenDoor()
{
	Open = true;
}

void ULockedDoor::CloseDoor()
{
	Open = false;
}

void ULockedDoor::SetPuzzleSolved()
{
	PuzzleSolved = true;
}

FBufferArchive ULockedDoor::GetSaveData()
{
	FBufferArchive BinaryData;

	int Op = (int)Open;
	int PSolved = (int)PuzzleSolved;
	FRotator IRotation = InitialRotation;
	float CRotation = CurrentRotation;

	BinaryData << Op;
	BinaryData << PSolved;
	BinaryData << IRotation;
	BinaryData << CRotation;
	BinaryData << DoorOpenCondition;

	return BinaryData;
}

void ULockedDoor::LoadSaveData(FMemoryReader &Ar)
{	
	int Op;
	int PSolved;
	FRotator IRotation;
	float CRotation;

	Ar << Op;
	Ar << PSolved;
	Ar << IRotation;
	Ar << CRotation;
	Ar << DoorOpenCondition;

	if (Op > 0)
		Open = true;
	if (Op == 0)
		Open = false;
	if (PSolved > 0)
		PuzzleSolved = true;
	if (PSolved == 0)
		PuzzleSolved = false;
	
	InitialRotation = IRotation;
	CurrentRotation = CRotation;

}

FVector ULockedDoor::GetActivatePosition(AActor* Player)
{
	// Vanskelig å forklare, men det den gjør er å finne en posisjon sånn at hvis spilleren åpner en dør
	// Så slår den han ikke i fjeset og ødelegger for kameraet.

	FVector ActorLocation = GetOwner()->GetActorLocation();

	FRotator Rotation = GetOwner()->GetActorRotation();

	FVector Rot = Rotation.Vector();

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	FVector Bounds = MeshComponent->GetStaticMesh()->GetBounds().BoxExtent;

	FVector PlayerPosition = Player->GetActorLocation();
	FVector OurPosition = GetOwner()->GetActorLocation();

	FVector PlayerToUs = OurPosition - PlayerPosition;
	PlayerToUs.Normalize();
	
	float dotProd = FVector::DotProduct(PlayerToUs, OpenDir);

	float Angle = FMath::Acos(dotProd);

	if (Open)
	{
		if (Angle < 1.4f)
		{
			if (OpenDirection)
				ActorLocation += GetOwner()->GetActorForwardVector()*Bounds.X + GetOwner()->GetActorRightVector()*50.f;
			else
				ActorLocation += GetOwner()->GetActorForwardVector()*Bounds.X - GetOwner()->GetActorRightVector()*50.f;
		}
		else
		{
			if (OpenDirection)
				ActorLocation -= GetOwner()->GetActorForwardVector()*Bounds.X*2.5;
			else
				ActorLocation -= GetOwner()->GetActorForwardVector()*Bounds.X*2.5;
		}
	}
	else
	{
		if (Angle < 1.4f)
		{
			if (OpenDirection)
				ActorLocation -= GetOwner()->GetActorForwardVector()*Bounds.X - GetOwner()->GetActorRightVector()*50.f;
			else
				ActorLocation -= GetOwner()->GetActorRightVector()*Bounds.X + GetOwner()->GetActorForwardVector()*50.f;
		}
		else
		{
			if (OpenDirection)
				ActorLocation -= GetOwner()->GetActorRightVector()*Bounds.X*2.5;
			else
				ActorLocation += GetOwner()->GetActorRightVector()*Bounds.X*2.5;
		}
	}

	return ActorLocation;
}