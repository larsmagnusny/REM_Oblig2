// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LockedDoor.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"

ULockedDoor::ULockedDoor()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void ULockedDoor::BeginPlay()
{
	Super::BeginPlay();

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	AREM_Hud* Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport();
		}
	}

	GameMode->AddInteractableObject(GetOwner(), this);
	InitialRotation = GetOwner()->GetActorRotation();

	// Used to check which side of the door the player is, so it doesn't slam him in the face when he tries to open it!
	if (OpenDirection)
		OpenDir = GetOwner()->GetActorUpVector();
	else
		OpenDir = -GetOwner()->GetActorUpVector();
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

void ULockedDoor::ActivateObject(AActor* Player)
{
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		if (Open)
			CloseDoor();
		else
			OpenDoor();
	}
	else {
		ExamineObject(Player);
	}
}

void ULockedDoor::ExamineObject(AActor* Player)
{
	if (DoorOpenCondition == OpenCondition::OPEN_NORMAL)
	{
		print("The door is unlocked, why not open it?!");
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
	
	float dotProd = FVector::DotProduct(PlayerToUs, OpenDir);

	float Angle = FMath::Acos(dotProd);
	

	UE_LOG(LogTemp, Warning, TEXT("Angle: %s"), *FString::SanitizeFloat(Angle));

	if (Open)
	{
		if (Angle < 1.4f)
		{
			if (OpenDirection)
				ActorLocation -= OpenDir*50.f;
			else
				ActorLocation -= OpenDir*50.f;
		}
		else
			ActorLocation += OpenDir*Bounds.Y;
	}
	else
	{
		if (Angle < 1.4f)
		{
			if (OpenDirection)
				ActorLocation += Rot*Bounds.X - OpenDir*50.f;
			else
				ActorLocation += Rot*Bounds.X - OpenDir*50.f;
		}
		else
		{
			if (OpenDirection)
				ActorLocation += Rot*Bounds.X + OpenDir*100.f;
			else
				ActorLocation += Rot*Bounds.X + OpenDir*100.f;
		}
	}
	
	DrawDebugSphere(GetWorld(), ActorLocation, 10.f, 5, FColor(255, 0, 0, 255), true, 5.0f, 0, 1.0f);

	return ActorLocation;
}