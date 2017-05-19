// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

// NOT IN USE

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

	CreatePossibleSolution(4);

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
		AREM_Hud* Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());
		Hud->canPlayerClick = false;
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

void UBookPuzzle::CreatePossibleSolution(int size)
{
	this->size = size;
	PossibleSolution = new int*[size];

	for (int i = 0; i < size; i++)
	{
		PossibleSolution[i] = new int[size];
	}

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			PossibleSolution[i][j] = 0;

	int value = FMath::RandRange(1, 5);

	for (int row = 0; row < size; row++)
	{
		for (int column = 0; column < size; column++)
		{
			if (ExistsAnyOtherInColumn(row, value, 0) || ExistsAnyOtherInRow(column, value, 0))
			{
				if (value > size + 1)
				{
					UE_LOG(LogTemp, Error, TEXT("Too big! row: %s column: %s"), *FString::FromInt(row), *FString::FromInt(column));
				}
				value++;
				column--;
			}
			else {
				PossibleSolution[row][column] = value;
				value = 1;
			}
		}
	}
}

bool UBookPuzzle::ExistsAnyOtherInRow(int column, int value, int AllowedOccurances)
{
	int Occurances = 0;
	for (int i = 0; i < size; i++)
	{
		// Check if there are any other numbers on this row that match
		if (PossibleSolution[i][column] == value)
			Occurances++;
	}

	if (Occurances == AllowedOccurances)
		return false;
	else
		return true;
}

bool UBookPuzzle::ExistsAnyOtherInRow(int column, int value, int AllowedOccurances, int** ArrayToCheck)
{
	int Occurances = 0;
	for (int i = 0; i < size; i++)
	{
		// Check if there are any other numbers on this row that match
		if (ArrayToCheck[i][column] == value)
			Occurances++;
	}

	if (Occurances == AllowedOccurances)
		return false;
	else
		return true;
}

bool UBookPuzzle::ExistsAnyOtherInColumn(int row, int value, int AllowedOccurances)
{
	int Occurances = 0;
	for (int i = 0; i < size; i++)
	{
		if (PossibleSolution[row][i] == value)
			Occurances++;
	}

	if (Occurances == AllowedOccurances)
		return false;
	else
		return true;
}

bool UBookPuzzle::ExistsAnyOtherInColumn(int row, int value, int AllowedOccurances, int** ArrayToCheck)
{
	int Occurances = 0;
	for (int i = 0; i < size; i++)
	{
		if (ArrayToCheck[row][i] == value)
			Occurances++;
	}

	if (Occurances == AllowedOccurances)
		return false;
	else
		return true;
}

void UBookPuzzle::RandomSlotToShow(int & column, int & row, int size)
{
	column = FMath::RandRange(0, size - 1);
	row = FMath::RandRange(0, size - 1);
}

bool UBookPuzzle::CheckSolution(int size, TArray<int> Array)
{
	// If any slot is 0 then its failed...
	for (int i = 0; i < Array.Num(); i++)
	{
		if (Array[i] == 0)
			return false;
	}


	int** TestSolution = new int*[size];

	for (int i = 0; i < size; i++)
	{
		TestSolution[i] = new int[size];
	}

	int YCounter = 0;

	for (int i = 0; i < Array.Num(); i++)
	{
		int XCounter = i - YCounter*size;

		if (XCounter == size)
		{
			YCounter++;
			XCounter = i - YCounter*size;
		}

		TestSolution[YCounter][XCounter] = Array[i];
		UE_LOG(LogTemp, Error, TEXT("Size: %s, Row: %s, Column: %s, Value: %s"), *FString::FromInt(size), *FString::FromInt(YCounter), *FString::FromInt(XCounter), *FString::FromInt(Array[i]));
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (ExistsAnyOtherInColumn(i, TestSolution[i][j], 1, TestSolution) || ExistsAnyOtherInRow(j, TestSolution[i][j], 1, TestSolution))
			{
				UE_LOG(LogTemp, Error, TEXT("Found Error in Row: %s, Column: %s"), *FString::FromInt(i), *FString::FromInt(j));
				return false;
			}
		}
	}

	return true;
}

int UBookPuzzle::GetSlotValueFromGrid(int column, int row)
{
	return PossibleSolution[row][column];
}

void UBookPuzzle::FromRowAndColumnToIndex(int & index, int column, int row, int size)
{
	index = column + row*size;
}

void UBookPuzzle::FromIndexToRowAndColumn(int index, int & column, int & row, int size)
{
	int r = 0;
	int c = 0;

	while (index > size)
	{
		index -= size;
		r++;
	}

	c = index;
	column = c;
	row = r;
}
