// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LookAtCamera.h"
#include "MainCharacter.h"

ALookAtCamera::ALookAtCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALookAtCamera::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	// Set the original Rotation of the Camera
	OrigRotation = GetActorRotation();
	OrigPosition = GetActorLocation();

	if (MainCamera)
	{
		AMainCharacter* OurCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

		if(OurCharacter)
			OurCharacter->ChangeCameraView(this);
	}
}

void ALookAtCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!LookAt)
	{
		LookAt = GameMode->GetMainCharacter();
		return;
	}

	FVector LookAtPosition = LookAt->GetActorLocation();
	FVector OurPosition = GetActorLocation();

	FVector Dir = LookAtPosition - OurPosition;

	FRotator EventualRotation = GetActorForwardVector().Rotation();

	FVector CameraNorm = OrigRotation.Vector();

	float Angle = FMath::Acos((FVector::DotProduct(Dir, CameraNorm)) / (Dir.Size()*CameraNorm.Size()));

	float NormLen = FMath::Cos(Angle)*Dir.Size();
	FVector Norm = CameraNorm*NormLen;

	float LenFromCenter = FMath::Sin(Angle)*Dir.Size();

	if (LenFromCenter > KeepCameraStillInThisRadius)
	{
		FVector Pos1 = GetActorLocation() + Dir;
		FVector Pos2 = GetActorLocation() + Norm;
		FVector FromCenterVec = Pos1 - Pos2;
		FVector LookAtPos = Pos2 + FromCenterVec;
		FromCenterVec.Normalize();
		LookAtPos -= FromCenterVec*KeepCameraStillInThisRadius;
		FVector LookAt = LookAtPos - GetActorLocation();

		EventualRotation = LookAt.Rotation();
	}

	if (LockX)
		EventualRotation.Roll = OrigRotation.Roll;
	if (LockY)
		EventualRotation.Pitch = OrigRotation.Pitch;
	if (LockZ)
		EventualRotation.Yaw = OrigRotation.Yaw;

	if (EventualRotation.Roll > MaxRoll)
		EventualRotation.Roll = MaxRoll;
	if (EventualRotation.Roll < MinRoll)
		EventualRotation.Roll = MinRoll;

	if (EventualRotation.Pitch > MaxPitch)
		EventualRotation.Pitch = MaxPitch;
	if (EventualRotation.Pitch < MinPitch)
		EventualRotation.Pitch = MinPitch;

	if (EventualRotation.Yaw > MaxYaw)
		EventualRotation.Yaw = MaxYaw;
	if (EventualRotation.Yaw < MinYaw)
		EventualRotation.Yaw = MinYaw;

	if (FollowCharacter)
	{
		FVector NormDir = LookAtPosition - OrigPosition;

		FVector FollowVector = FVector(0, 0, 0);
		if (FollowAlongAxisY)
		{
			float YValue = NormDir.Y;

			if (YValue + FollowDistance > 0)
			{
				YValue = -FollowDistance;
			}

			FollowVector = FVector(0, FollowDistance + YValue, 0);

			if (FMath::Abs(YValue) > MaxFollowDistance)
				FollowVector = FVector(0, FollowDistance + MaxFollowDistance*(YValue/(FMath::Abs(YValue))), 0);

			

			SetActorLocation(OrigPosition + FollowVector);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *FollowVector.ToString());
		}

		if (FollowAlongAxisX)
		{
			float XValue = NormDir.X;

			if (FMath::Abs(XValue) > MaxFollowDistance)
				XValue = MaxFollowDistance;

			FollowVector += FVector(0, FollowDistance + XValue, 0);
		}
		
		SetActorRotation(EventualRotation.Quaternion());
	}
}