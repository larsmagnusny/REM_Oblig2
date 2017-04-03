// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Book.h"
#include "BookCase.h"

UBook::UBook()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBook::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	OriginalPosition = GetOwner()->GetActorLocation();
	OriginalRotation = GetOwner()->GetActorRotation();

	CurrentPosition = OriginalPosition;

	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UBook::BeginOverlap);
}

void UBook::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetOwner()->SetActorLocation(CurrentPosition);

	if (!CanOverlap)
		CanOverlap = true;
}

void UBook::ActivateObject(AActor * Player)
{
	UBookCase* BookCase = Cast<UBookCase>(ParentComponent);

	if (BookCase->DraggingBook)
	{
		BookCase->DraggingBook = false;
		BookCase->BookToDrag = nullptr;
		CurrentPosition = BookCase->GetPositionFromSlot(OccupyingIndex);
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
	UE_LOG(LogTemp, Error, TEXT("Save: %s"), *FString::FromInt(OccupyingIndex));
	return BinaryData;
}

void UBook::LoadSaveData(FMemoryReader & Ar)
{
	Ar << OccupyingIndex;

	//CurrentPosition = Cast<UBookCase>(ParentComponent)->GetPositionFromSlot(OccupyingIndex);

	
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

void UBook::BeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	//if (!OtherActor->IsA(UBook::StaticClass()))
		//return;

	if (!CanOverlap)
		return;

	UBookCase* Parent = Cast<UBookCase>(ParentComponent);

	if (Cast<UBookCase>(ParentComponent)->BookToDrag != MyOverlappedActor && Parent->GetBookComponentByActor(OtherActor) && Parent->GetBookComponentByActor(MyOverlappedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Being Overlapped!"));
		

		UBook* OtherComponent = Parent->GetBookComponentByActor(OtherActor);

		int OtherIndex = OtherComponent->OccupyingIndex;

		int index1 = OtherIndex;
		int index2 = OccupyingIndex;
		OtherComponent->OccupyingIndex = OccupyingIndex;
		OccupyingIndex = OtherIndex;

		CurrentPosition = Parent->GetPositionFromSlot(OccupyingIndex);
	}
}
