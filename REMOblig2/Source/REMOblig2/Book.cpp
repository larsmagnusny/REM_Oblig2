// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Book.h"
#include "BookCase.h"

UBook::UBook()
{
	PrimaryComponentTick.bCanEverTick = true;
	CanRightClick = false;
}

void UBook::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	OriginalPosition = GetOwner()->GetActorLocation();
	OriginalRotation = GetOwner()->GetActorRotation();

	CurrentPosition = OriginalPosition;
}

void UBook::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UBookCase* BookCase = Cast<UBookCase>(ParentComponent);

	if(BookCase->PuzzleSolved)
		PrimaryComponentTick.bCanEverTick = false;

	if (Wait)
	{
		if (TimeCounter > TimeToWait)
		{
			TimeCounter = 0.f;
			Wait = false;
			CanOverlap = true;
		}
		else {
			TimeCounter += DeltaTime;
		}
	}

	GetOwner()->SetActorLocation(CurrentPosition);

	TArray<AActor*> OverlappedActors;
	GetOwner()->GetOverlappingActors(OverlappedActors);

	for (AActor* OtherActor : OverlappedActors)
	{
		UBook* OtherBook = Cast<UBook>(OtherActor->GetComponentByClass(UBook::StaticClass()));

		if (BookCase->BookToDrag == GetOwner())
			OtherBook->Overlap(OtherActor, GetOwner());
		else
			Overlap(GetOwner(), OtherActor);
	}

	if (!CanOverlap)
	{
		Wait = true;
	}
}

void UBook::ActivateObject(AActor * Player)
{
	UBookCase* BookCase = Cast<UBookCase>(ParentComponent);

	if (BookCase->DraggingBook)
	{
		UBook* OtherComponent = BookCase->GetBookComponentByActor(BookCase->BookToDrag);
		OtherComponent->CurrentPosition = BookCase->GetPositionFromSlot(OtherComponent->OccupyingIndex);

		BookCase->DraggingBook = false;
		BookCase->BookToDrag = nullptr;
	}
	else
	{
		BookCase->DraggingBook = true;
		BookCase->BookToDrag = GetOwner();
	}
}

FVector UBook::GetActivatePosition(AActor * Player)
{
	return FVector();
}

FBufferArchive UBook::GetSaveData()
{
	FBufferArchive BinaryData;
	BinaryData << OccupyingIndex;
	return BinaryData;
}

void UBook::LoadSaveData(FMemoryReader & Ar)
{
	Ar << OccupyingIndex;

	CanOverlap = false;
	CurrentPosition = Cast<UBookCase>(ParentComponent)->GetPositionFromSlot(OccupyingIndex);
	GetOwner()->SetActorLocation(CurrentPosition);
}

void UBook::SetInteractable()
{
	GameMode->AddInteractableObject(GetOwner(), this);
}

void UBook::UnsetInteractable()
{
	GameMode->RemoveInteractableObject(GetOwner());
}

void UBook::SetParent(UInteractableComponent * Parent)
{
	ParentComponent = Parent;
}

void UBook::Overlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (!CanOverlap)
	{
		CanOverlap = true;
		return;
	}

	UBookCase* Parent = Cast<UBookCase>(ParentComponent);

	if (Cast<UBookCase>(ParentComponent)->BookToDrag != MyOverlappedActor && Parent->GetBookComponentByActor(OtherActor) && Parent->GetBookComponentByActor(MyOverlappedActor))
	{
		UBook* OtherComponent = Parent->GetBookComponentByActor(OtherActor);

		int OtherIndex = OtherComponent->OccupyingIndex;

		int index1 = OtherIndex;
		int index2 = OccupyingIndex;
		OtherComponent->OccupyingIndex = OccupyingIndex;
		OccupyingIndex = OtherIndex;

		CurrentPosition = Parent->GetPositionFromSlot(OccupyingIndex);
	}
}
