// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "ImageExamine.h"

UImageExamine::UImageExamine()
{
	PrimaryComponentTick.bCanEverTick = true;

	CanRightClick = false;
}

void UImageExamine::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Make the object clickable, but do not show a menu...
	GameMode->AddInteractableObject(GetOwner(), this);
}

void UImageExamine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!CurrentCamera)
		return;

	

	if (RunCameraAnimation)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (Forward)
		{
			int moveSpeed = 8;

			FVector Add = FMath::VInterpTo(CurrentCamera->GetActorLocation(), MainCameraFinalPosition, DeltaTime, moveSpeed);

			FRotator Rot = FMath::VInterpNormalRotationTo(CurrentCamera->GetActorRotation().Vector(), MainCameraFinalRotation.Vector(), DeltaTime, moveSpeed*RotateMultiplier).ToOrientationRotator();

			CurrentCamera->SetActorLocation(Add);
			CurrentCamera->SetActorRotation(Rot);

			float Dist = FVector::Dist(CurrentCamera->GetActorLocation(), MainCameraFinalPosition);

			if (Dist < 1.f)
			{
				RunCameraAnimation = false;

				CurrentCamera->SetActorLocation(MainCameraFinalPosition);
				CurrentCamera->SetActorRotation(MainCameraFinalRotation);

				Hud->canPlayerClick = true;
				Cast<AMainCharacter>(MCharacter)->IsInPuzzleGameMode = true;

				Forward = false;
			}
		}
		else
		{
			int moveSpeed = 8;

			FVector Add = FMath::VInterpTo(CurrentCamera->GetActorLocation(), MainCameraOrigPosition, DeltaTime, moveSpeed);
			FRotator Rot = FMath::VInterpNormalRotationTo(CurrentCamera->GetActorRotation().Vector(), MainCameraOrigRotation.Vector(), DeltaTime, moveSpeed*16.3f).ToOrientationRotator();

			CurrentCamera->SetActorLocation(Add);
			CurrentCamera->SetActorRotation(Rot);

			float Dist = FVector::Dist(CurrentCamera->GetActorLocation(), MainCameraOrigPosition);

			if (Dist < 1.f)
			{
				CurrentCamera->SetActorLocation(MainCameraOrigPosition);
				CurrentCamera->SetActorRotation(MainCameraOrigRotation);
				RunCameraAnimation = false;

				Forward = true;

				if (ResetFollowCharacter)
				{
					CurrentCamera->FollowCharacter = true;
					ResetFollowCharacter = false;
				}

				Cast<AMainCharacter>(MCharacter)->IsInPuzzleGameMode = false;
			}
		}
	}
}

void UImageExamine::ActivateObject(AActor * Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Ran ActivateObject on Image!"));
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(Player);

	MCharacter = MainCharacter;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	CameraManager = PlayerController->PlayerCameraManager;

	CurrentCamera = Cast<ALookAtCamera>(CameraManager->GetViewTarget());

	if (CurrentCamera->FollowCharacter)
	{
		CurrentCamera->FollowCharacter = false;
		ResetFollowCharacter = true;
	}

	if (!OrigPositionAndRotationSet)
	{
		MainCameraOrigPosition = CurrentCamera->GetActorLocation();
		MainCameraOrigRotation = CurrentCamera->GetActorRotation();
		OrigPositionAndRotationSet = true;
	}

	if (!Forward)
	{
		GameMode->AddInteractableObject(GetOwner(), this);
		MainCharacter->IsInPuzzleGameMode = false;
	}
	else
	{
		MainCharacter->IsInPuzzleGameMode = true;
		GameMode->RemoveInteractableObject(GetOwner());
	}

	RunCameraAnimation = true;
}

FVector UImageExamine::GetActivatePosition(AActor * Player)
{
	return GetOwner()->GetActorLocation();
}
