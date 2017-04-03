// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "BookCase.h"
#include "MainCharacter.h"


// Sets default values for this component's properties
UBookCase::UBookCase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBookCase::BeginPlay()
{
	Super::BeginPlay();

	// Legg til meny-valg for "Right-Click" Meny
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// SubMenuWidgetClass definert i UInteractableComponent
	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport();
		}
	}

	// Fortell gamemode at denne er interactable

	GameMode->AddInteractableObject(GetOwner(), this);
	
	// Lets add all the components of our registered books...
	for (int i = 0; i < Books.Num(); i++)
	{
		BookComponents.Add(Cast<UBook>(Books[i]->GetComponentByClass(UBook::StaticClass())));
		SnapPositions.Add(Books[i]->GetActorLocation());
	}

	int counter = 0;

	for (UBook* BookComp : BookComponents)
	{
		BookComp->SetParent(this);

		BookComp->OccupyingIndex = counter++;
	}
}


// Called every frame
void UBookCase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LoadGameInit)
	{
		LoadGameInit = false;

		for (UBook* BookComp : BookComponents)
		{
			BookComp->CanOverlap = false;
			BookComp->CurrentPosition = GetPositionFromSlot(BookComp->OccupyingIndex);

			UE_LOG(LogTemp, Error, TEXT("Load: %s Position: %s"), *FString::FromInt(BookComp->OccupyingIndex), *BookComp->CurrentPosition.ToString());
		}

		if (PuzzleSolved)
		{
			if (GameMode->IsInteractble(GetOwner()))
			{
				GameMode->RemoveInteractableObject(GetOwner());
			}
		}

	}

	if (!CanRunAnimation)
		return;

	if (ShouldWait)
	{
		TimeCounter += DeltaTime;

		if (TimeCounter > Wait)
		{
			ShouldWait = false;
		}
		return;
	}

	if (PuzzleSolved)
	{
		RunCameraAnimation = true;
	}

	if (TimeCounter > CheckSolutionInterval && !PuzzleSolved && !DraggingBook)
	{
		TimeCounter = 0.f;

		PuzzleSolved = CheckSolution();

		if (PuzzleSolved)
		{
			ShouldWait = true;
		}
	}

	if (DraggingBook)
	{
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		float MouseX, MouseY;

		Controller->GetMousePosition(MouseX, MouseY);

		FVector2D ScreenPos = FVector2D(MouseX, MouseY);

		FVector WorldPosition;

		FVector WorldDirection;

		UGameplayStatics::DeprojectScreenToWorld(Controller, ScreenPos, WorldPosition, WorldDirection);

		FVector OrigPosition = GetBookComponentByActor(BookToDrag)->OriginalPosition;

		FVector Right = BookToDrag->GetActorRightVector();
		FVector Left = BookToDrag->GetActorRightVector() * (-1.0f);

		FVector Up = BookToDrag->GetActorUpVector();
		
		FVector PlaneNormal = FVector::CrossProduct(Right, Up);

		// ax + by + bz = 0

		FVector Position = FMath::LinePlaneIntersection(WorldPosition, WorldDirection*1000.f, BookToDrag->GetActorLocation(), PlaneNormal);

		Position.Z = OrigPosition.Z;

		GetBookComponentByActor(BookToDrag)->CurrentPosition = Position + Left*3.5f;
	}


	if (StopCameraAnimation)
	{
		StopCameraAnimation = false;
		RunCameraAnimation = false;
	}

	if (RunCameraAnimation)
	{
		if (Forward)
		{
			FVector Direction = MainCameraFinalPosition - CurrentCamera->GetActorLocation();

			float Distance = Direction.Size();

			FRotator CurrentRotation = CurrentCamera->GetActorRotation();
			
			float CurrentPitch = CurrentRotation.Pitch;
			float CurrentYaw = CurrentRotation.Yaw;
			float CurrentRoll = CurrentRotation.Roll;

			FVector WantedRotation = FVector(MainCameraFinalRotation.Pitch, MainCameraFinalRotation.Yaw, MainCameraFinalRotation.Roll);
			FVector CRotation = FVector(CurrentPitch, CurrentYaw, CurrentRoll);

			FVector RotDir = WantedRotation - CRotation;

			float RotMag = RotDir.Size();

			if (RotMag > 0.5f)
			{
				RotDir.Normalize();

				RotDir *= 0.5f;

				FRotator NextRotation = FRotator(CurrentPitch + RotDir.X, CurrentYaw + RotDir.Y, CurrentRoll + RotDir.Z);

				CurrentCamera->SetActorRotation(NextRotation);
			}

			if (Distance > 10)
			{
				Direction.Normalize();

				CurrentCamera->SetActorLocation(CurrentCamera->GetActorLocation() + Direction*5.0f);
			}
			else {
				CurrentCamera->SetActorLocation(MainCameraFinalPosition);
				CurrentCamera->SetActorRotation(MainCameraFinalRotation);
				MakeAllBooksInteractable();
				RunCameraAnimation = false;
				Forward = false;
			}
		}
		else {
			FVector Direction = MainCameraOrigPosition - CurrentCamera->GetActorLocation();

			float Distance = Direction.Size();

			if (Distance > 10)
			{
				Direction.Normalize();

				CurrentCamera->SetActorLocation(CurrentCamera->GetActorLocation() + Direction*5.0f);

				CurrentCamera->FollowCharacter = true;
			}
			else
			{
				MakeAllBooksNonInteractable();
				CurrentCamera->SetActorLocation(MainCameraOrigPosition);
				CurrentCamera->SetActorRotation(MainCameraOrigRotation);
				RunCameraAnimation = false;
				Forward = true;

				if (PuzzleSolved)
				{
					CanRunAnimation = false;
					if (GameMode->IsInteractble(GetOwner()))
					{
						GameMode->RemoveInteractableObject(GetOwner());
					}

					// Spawn an inventoryitemobject here...
					UStaticMesh* Mesh = GameMode->MeshesAndTextures->GetStaticMeshByItemID(ItemToDrop);
					UTexture2D* Texture = GameMode->MeshesAndTextures->GetTextureByItemID(ItemToDrop);
					InventoryItem* Item = new InventoryItem(ItemToDrop, InteractID, "Toy", Mesh, Texture);

					// Spawn ett objekt
					GameMode->PutObjectInWorld(Item, PositionToDrop, FVector(0, 0, 0), FVector(1, 1, 1));

				}
				else {
					if(!GameMode->IsInteractble(GetOwner()))
						GameMode->AddInteractableObject(GetOwner(), this);
				}

				Cast<AMainCharacter>(MCharacter)->IsInPuzzleGameMode = false;
			}
		}
	}

	TimeCounter += DeltaTime;
}

void UBookCase::ActivateObject(AActor * Player)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MCharacter = MainCharacter;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	CameraManager = PlayerController->PlayerCameraManager;

	CurrentCamera = Cast<ALookAtCamera>(CameraManager->GetViewTarget());

	if (!OrigPositionAndRotationSet)
	{
		MainCameraOrigPosition = CurrentCamera->GetActorLocation();
		MainCameraOrigRotation = CurrentCamera->GetActorRotation();
		OrigPositionAndRotationSet = true;
	}

	if (!Forward)
	{
		CurrentCamera->FollowCharacter = true;
		GameMode->AddInteractableObject(GetOwner(), this);
		MainCharacter->IsInPuzzleGameMode = false;
	}
	else
	{
		MainCharacter->IsInPuzzleGameMode = true;
		CurrentCamera->FollowCharacter = false;
		GameMode->RemoveInteractableObject(GetOwner());
	}
	RunCameraAnimation = true;
}

void UBookCase::ExamineObject(AActor * Player)
{
}

FVector UBookCase::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation();
}

FBufferArchive UBookCase::GetSaveData()
{
	FBufferArchive BinaryData;

	BinaryData << CanRunAnimation;
	BinaryData << PuzzleSolved;

	UE_LOG(LogTemp, Error, TEXT("Saved: %s"), *FString::FromInt(CanRunAnimation));
	UE_LOG(LogTemp, Error, TEXT("Saved: %s"), *FString::FromInt(PuzzleSolved));

	return BinaryData;
}

void UBookCase::LoadSaveData(FMemoryReader & Ar)
{
	Ar << CanRunAnimation;
	Ar << PuzzleSolved;
}

void UBookCase::MakeAllBooksInteractable()
{
	for (int i = 0; i < BookComponents.Num(); i++)
	{
		if (BookComponents[i])
		{
			BookComponents[i]->SetInteractable();
		}
	}
}

void UBookCase::MakeAllBooksNonInteractable()
{
	for (int i = 0; i < BookComponents.Num(); i++)
	{
		if (BookComponents[i])
		{
			BookComponents[i]->UnsetInteractable();
		}
	}
}

FVector UBookCase::GetPositionFromSlot(int Index)
{
	return SnapPositions[Index];
}

void UBookCase::SwapPositions(int index1, int index2)
{
	//BookComponents.Swap(index1, index2);

	//Books.Swap(index1, index2);
}

UBook * UBookCase::GetBookComponentByActor(AActor * BookObject)
{
	for (int i = 0; i < Books.Num(); i++)
	{
		if (Books[i] == BookObject)
			return BookComponents[i];
	}

	return nullptr;
}

bool UBookCase::CheckSolution()
{
	TArray<FString> Array3;
	TArray<int> IndexArray;


	for (UBook* Book : BookComponents)
	{
		IndexArray.Add(Book->OccupyingIndex);
		Array3.Add(Book->BookTitle);
	}

	TArray<FString> Array;
	Array.SetNum(IndexArray.Num(), false);

	for (int i = 0; i < IndexArray.Num(); i++)
	{
		Array[IndexArray[i]] = Array3[i];
	}

	TArray<FString> Array2 = Array;

	Array.Sort();

	for (int i = 0; i < Array.Num(); i++)
	{
		if (!Array[i].Equals(Array2[i]))
		{
			return false;
		}
	}



	return true;
}

