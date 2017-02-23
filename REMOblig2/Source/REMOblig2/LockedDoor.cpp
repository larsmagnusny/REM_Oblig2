// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "REMOblig2.h"
#include "LockedDoor.h"
#include "REM_GameMode.h"

ULockedDoor::ULockedDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockedDoor::BeginPlay()
{
	Super::BeginPlay();
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	GameMode->AddInteractableObject(GetOwner(), this);
	InitialRotation = GetOwner()->GetActorRotation();
}

void ULockedDoor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

void ULockedDoor::ActivateObject()
{
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		if (Open)
			CloseDoor();
		else
			OpenDoor();
	}
	if (DoorOpenCondition == OpenCondition::OPEN_KEY)
	{
		print("The door is locked. There must be a key somewhere.");
	}
	if (DoorOpenCondition == OpenCondition::OPEN_COMPLETE_PUZZLE)
	{
		print("The door is locked, there's no keyhole.");
	}
	if (DoorOpenCondition == OpenCondition::OPEN_TRIGGER)
	{
		print("There seems to be wires connected to the door, it leads to the pressurepad.");
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

void ULockedDoor::UnlockDoor(InventoryItem* item)
{

}

void ULockedDoor::LockDoor(InventoryItem* item)
{

}

void ULockedDoor::SetPuzzleSolved()
{
	PuzzleSolved = true;
}

FVector ULockedDoor::GetActivatePosition(AActor* Player)
{
	//Super::GetActivatePosition();
	FVector ActorLocation = GetOwner()->GetActorLocation();

	FRotator Rotation = GetOwner()->GetActorRotation();

	FVector Rot = Rotation.Vector();
	//Rot.X = 0.f;
	//Rot.Y = 0.f;

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	FVector Bounds = MeshComponent->GetStaticMesh()->GetBounds().BoxExtent;

	FVector PlayerPosition = Player->GetActorLocation();
	FVector OurPosition = GetOwner()->GetActorLocation();

	FVector PlayerToUs = OurPosition - PlayerPosition;
	PlayerToUs.Normalize();

	FVector OpenDir;



	if (OpenDirection)
		OpenDir = GetOwner()->GetActorUpVector();
	else
		OpenDir = -GetOwner()->GetActorUpVector();
	float dotProd = FVector::DotProduct(PlayerToUs, OpenDir);

	float Angle = FMath::Acos(dotProd);
	

	UE_LOG(LogTemp, Warning, TEXT("Angle: %s"), *FString::SanitizeFloat(Angle));

	if (Open)
	{
		if (Angle < 1.2f)
			ActorLocation -= Rot*50.f;
		else
			ActorLocation += Rot*Bounds.Y;
	}
	else
	{
		if (Angle < 1.15f)
			ActorLocation += Rot*Bounds.X + GetOwner()->GetActorUpVector()*50.f;
		else
			ActorLocation += Rot*Bounds.X - GetOwner()->GetActorUpVector()*100.f;
	}
	
	DrawDebugSphere(GetWorld(), ActorLocation, 50.f, 32, FColor(255, 0, 0, 255), true, 5.0f, 0, 1.0f);

	return ActorLocation;
}