// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "BookPuzzle.h"
#include "MainCharacter.h"

UBookPuzzle::UBookPuzzle()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetLoader(TEXT("WidgetBlueprint'/Game/UI/BookPuzzle.BookPuzzle_C'"));

	if (WidgetLoader.Succeeded())
	{
		PuzzleWidgetClassTemplate = WidgetLoader.Class;
	}
}

void UBookPuzzle::BeginPlay()
{
	Super::BeginPlay();

	// Legg til Meny-valg dynamisk så vi slipper å lage en ny widget for hver eneste item
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	// Hent en peker til GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (PuzzleWidgetClassTemplate)
	{
		PuzzleWidget = Hud->HUDCreateWidget(PuzzleWidgetClassTemplate);

		if (PuzzleWidget)
		{
			Hud->BookPuzzleParent = Cast<UActorComponent>(this);
			PuzzleWidget->AddToViewport(10);
			PuzzleWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

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

void UBookPuzzle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasPuzzleBeenSolved)
	{
		if (PuzzleSolved)
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());
			TArray<FString> Conversation;

			InventoryItem* Item = new InventoryItem(ItemIDs::ITEM_SPINNER, 666, "SpinnerToy", GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemIDs::ITEM_SPINNER), GameMode->MeshesAndTextures->GetTextureByItemID(ItemIDs::ITEM_SPINNER));


			GameMode->PutObjectInWorld(Item, GetOwner()->GetActorLocation(), FVector::ZeroVector, FVector(1, 1, 1));

			Conversation.Add("Good job.");

			MainCharacter->Conversation = Conversation;
			MainCharacter->ShouldShowConversation = true;
			MainCharacter->SetDialogueChoiceVisible();

			HasPuzzleBeenSolved = true;
		}
	}
	else {
		if (GameMode->IsInteractble(GetOwner()))
		{
			GameMode->RemoveInteractableObject(GetOwner());
		}
	}
}

void UBookPuzzle::ActivateObject(AActor * Player)
{
	if (PuzzleWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		PuzzleWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBookPuzzle::ExamineObject(AActor * Player)
{
}

void UBookPuzzle::SolvePuzzle()
{
	PuzzleSolved = true;
}

FBufferArchive UBookPuzzle::GetSaveData()
{
	FBufferArchive BinaryData;
	BinaryData << PuzzleSolved;
	BinaryData << HasPuzzleBeenSolved;
	return BinaryData;
}

void UBookPuzzle::LoadSaveData(FMemoryReader & Ar)
{
	Ar << PuzzleSolved;
	Ar << HasPuzzleBeenSolved;
}
