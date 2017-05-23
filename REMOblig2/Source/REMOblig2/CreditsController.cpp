// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "CreditsController.h"


// Sets default values
ACreditsController::ACreditsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Names.Add("Mathilde Kvernland\nRebecca Waagoones Losendahl"); // 0
	Names.Add("Mathilde Kvernland"); // 1
	Names.Add("Malin Spilde"); // 2
	Names.Add("Rikke Loover Cedervall"); // 3
	Names.Add("Rikke Loover Cedervall\nMalin Spilde"); // 4
	Names.Add("Rikke Loover Cedervall\nMalin Spilde"); // 5
	Names.Add("Rikke Loover Cedervall\nRebecca Waagoones Losendahl"); // 6
	Names.Add("Jostein Risto"); // 7
	Names.Add("Lars Magnus Nyland\nUne Haavik Johnsen"); // 8
}

// Called when the game starts or when spawned
void ACreditsController::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	GameInstance = GameMode->GameInstance;
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	Hud->CreditsActor = this;

	GameInstance->CreditsLevel = true;

	MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	CameraManager = PlayerController->PlayerCameraManager;

	CurrentCamera = Cast<ALookAtCamera>(CameraManager->GetViewTarget());

	MainCameraOrigPosition = CurrentCamera->GetActorLocation();
	MainCameraOrigRotation = CurrentCamera->GetActorRotation();

	MainCameraFinalPosition = CameraPositions[0];
	MainCameraFinalRotation = CameraRotations[0];

	CurrentRole = GroupRole[0];
	CurrentName = Names[0];

	GameInstance->SFXVolume = 0.f;

	CameraCounter++;
}

// Called every frame
void ACreditsController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!CurrentCamera)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("Time: %s"), *FString::SanitizeFloat(WaitCounter));

	if (WaitCounter < WaitTime)
	{
		WaitCounter += DeltaTime;
		return;
	}
	else {
		UChestController* ChestController = Cast<UChestController>(ChestActor->GetComponentByClass(UChestController::StaticClass()));

		
		if (CameraCounter == 6 && !ChestController->RunStandupAnimation)
		{
			// Make chest stand up...
			if (ChestActor)
			{
				ChestController->RunStandupAnimation = true;
			}
		}
	}

	if (CreditsDone)
	{
		GameInstance->LoadCheckpointNextLevel = false;
		GameInstance->CreditsLevel = false;
		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	float Dist = FVector::Dist(MainCameraFinalPosition, MainCameraOrigPosition);


	float moveSpeed = Dist*(1.f / TransitionTime)*0.02f;

	FVector OrigVec = MainCameraOrigRotation.Vector();
	FVector WantedVec = MainCameraFinalRotation.Vector();

	float DeltaAngle = FMath::Acos(FVector::DotProduct(OrigVec, WantedVec)) * (180.f / 3.141592f);

	float rotateSpeed = FMath::Abs(DeltaAngle*(1.f / TransitionTime)*1.2f);

	

	FVector Add = FMath::VInterpTo(CurrentCamera->GetActorLocation(), MainCameraFinalPosition, DeltaTime, moveSpeed);

	FRotator Rot = FMath::VInterpNormalRotationTo(CurrentCamera->GetActorRotation().Vector(), MainCameraFinalRotation.Vector(), DeltaTime, rotateSpeed).ToOrientationRotator();

	CurrentCamera->SetActorLocation(Add);
	CurrentCamera->SetActorRotation(Rot);

	Dist = FVector::Dist(CurrentCamera->GetActorLocation(), MainCameraFinalPosition);

	UE_LOG(LogTemp, Error, TEXT("Dist: %s"), *FString::SanitizeFloat(Dist));

	if (Dist < 0.2f)
	{
		UE_LOG(LogTemp, Error, TEXT("NEXT POSITION!"));
		CurrentCamera->SetActorLocation(MainCameraFinalPosition);
		CurrentCamera->SetActorRotation(MainCameraFinalRotation);

		MainCameraOrigPosition = CurrentCamera->GetActorLocation();
		MainCameraOrigRotation = CurrentCamera->GetActorRotation();

		if (CameraCounter < CameraPositions.Num() - 1)
		{
			CurrentRole = GroupRole[CameraCounter-1];
			CurrentName = Names[CameraCounter-1];
			CameraCounter++;
			UE_LOG(LogTemp, Error, TEXT("%s"), *FString::FromInt(CameraCounter));

			MainCameraFinalPosition = CameraPositions[CameraCounter];
			MainCameraFinalRotation = CameraRotations[CameraCounter];
			WaitCounter = 0.f;
			ShouldShowNext = true;
			
		}
		else
		{
			WaitCounter = 0.f;
			WaitTime = 7.0f;
			ShouldShowNext = true;
			CurrentRole = "Special thanks for guidance to";
			CurrentName = "Einar Langjord (UV-mapping, and animation)\nJonas Hagemo(Animation)\nMarkus Storeide(Rigging)\nFred Froolich\nDaniel Kim\nWenche Hellekaas\nHaavard Vibeto\nOle Flaten";
			CreditsDone = true;
		}
	}
}

