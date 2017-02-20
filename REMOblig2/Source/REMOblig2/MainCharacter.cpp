<<<<<<< HEAD
// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the player inventory:
	PlayerInventory = new Inventory(4);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	if (!CameraBoom)
	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->bAbsoluteRotation = true;
		CameraBoom->TargetArmLength = 380.0f;
		CameraBoom->RelativeRotation = FRotator(-60.0f, 145.0f, 0.0f);
		CameraBoom->bDoCollisionTest = true;

		if (!TopDownCameraComponent)
		{
			TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
			TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
			TopDownCameraComponent->bUsePawnControlRotation = false;
		}
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Load in the mesh if it has not been set...
	if (!SkeletalMeshComponent)
	{
		SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshLoader(TEXT("SkeletalMesh'/Game/Meshes/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

		if (SkeletalMeshLoader.Succeeded())
		{
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshLoader.Object);
			SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
			SkeletalMeshComponent->SetRelativeRotation(FRotator(0, -90, 0));

			if (!AnimClass)
			{
				SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

				static ConstructorHelpers::FObjectFinder<UClass> AnimationBP(TEXT("Class'/Game/Meshes/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));

				if (AnimationBP.Succeeded())
				{
					SkeletalMeshComponent->SetAnimInstanceClass(AnimationBP.Object);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set the viewtarget of this camera to look at the Main Character
	GetWorld()->GetFirstPlayerController()->SetViewTarget(this);

	// Setup inputs for the character ( mouse for the moment )
	SetupPlayerInputComponent(GetWorld()->GetFirstPlayerController()->InputComponent);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->SetMainCamera(TopDownCameraComponent);

	// Make the gamemode aware that we exist:
	GameMode->SetMainCharacter(this);

	NavSys = GetWorld()->GetNavigationSystem();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Raycast under the mouse so we can highlight the objects
	FHitResult Hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit);

	if (Hit.GetActor())
	{
		if (GameMode->IsInteractble(Hit.GetActor()))
		{
			// Get a pointer to the actor and MeshComponent
			AStaticMeshActor* OurActor = Cast<AStaticMeshActor>(Hit.GetActor());
			UInteractableComponent* InteractableComponent = GameMode->GetInteractor(Hit.GetActor());
			AInteractableObject* InteractableObj = GameMode->GetStaticMeshInteractor(Hit.GetActor());

			UStaticMeshComponent* MeshComponent = nullptr;
			USkeletalMeshComponent* TSkeletalMeshComponent = nullptr;


			if (!InteractableComponent && InteractableObj)
			{
				MeshComponent = InteractableObj->GetStaticMeshComponent();
			}
			if (!InteractableObj && InteractableComponent)
			{
				MeshComponent = InteractableComponent->GetStaticMeshComponent();
				TSkeletalMeshComponent = InteractableComponent->GetSkeletalMeshComponent();
			}

			if (MeshComponent && !TSkeletalMeshComponent)
			{
				if (MeshComponent != LastComponentMousedOver)
				{
					if (LastComponentMousedOver)
						LastComponentMousedOver->SetRenderCustomDepth(false);
				}

				if (!MeshComponent->bRenderCustomDepth)
				{
					MeshComponent->SetRenderCustomDepth(true);
					LastComponentMousedOver = MeshComponent;
				}
			}
			if (!MeshComponent && TSkeletalMeshComponent)
			{
				if (TSkeletalMeshComponent != LastSkeletalMeshComponentMousedOver)
				{
					if (LastSkeletalMeshComponentMousedOver)
						LastSkeletalMeshComponentMousedOver->SetRenderCustomDepth(false);
				}

				if (!TSkeletalMeshComponent->bRenderCustomDepth)
				{
					TSkeletalMeshComponent->SetRenderCustomDepth(true);
					LastSkeletalMeshComponentMousedOver = TSkeletalMeshComponent;
				}
			}
		}
		else
		{
			if (LastComponentMousedOver)
				LastComponentMousedOver->SetRenderCustomDepth(false);
			if (LastSkeletalMeshComponentMousedOver)
				LastSkeletalMeshComponentMousedOver->SetRenderCustomDepth(false);

			LastSkeletalMeshComponentMousedOver = nullptr;
			LastComponentMousedOver = nullptr;
		}
	}
	else
	{
		if (LastComponentMousedOver)
			LastComponentMousedOver->SetRenderCustomDepth(false);

		if (LastSkeletalMeshComponentMousedOver)
			LastSkeletalMeshComponentMousedOver->SetRenderCustomDepth(false);

		LastSkeletalMeshComponentMousedOver = nullptr;
		LastComponentMousedOver = nullptr;
	}


	// If the player clicked on the screen somewhere...
	if (MouseMove)
	{
		// 2D distance to target...
		float const Distance = FVector2D::Distance(FVector2D(MoveTo), FVector2D(GetActorLocation()));
		if (Distance == lastDistance)
			MoveTo = GetActorLocation();
		lastDistance = Distance;


		if (NavSys && (Distance > 25.0f))
		{
			NavSys->SimpleMoveToLocation(Controller, MoveTo);
		}
		else
		{
			MouseMove = false;

			if (DelayClimb)
			{
				DelayClimb = false;
				UCapsuleComponent* OurCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
				float CapsuleHeight = OurCapsule->GetScaledCapsuleHalfHeight()*2.0f;
				FHitResult ObjectAbove;
				GetWorld()->LineTraceSingleByChannel(ObjectAbove, GetActorLocation(), GetActorLocation() + FVector::UpVector*CapsuleHeight * 3, ECollisionChannel::ECC_Visibility);

				if (!ObjectAbove.GetActor())
					SetActorLocation(ClimbTo, false, nullptr, ETeleportType::TeleportPhysics);
			}

			if (DelayActivate)
			{
				DelayActivate = false;
				if (DelayActivateObject.OwningActor)
				{
					if (DelayActivateObject.ScriptComponent)
					{
						DelayActivateObject.ScriptComponent->ActivateObject();
					}
					if (DelayActivateObject.StaticMeshInstance)
					{
						DelayActivateObject.StaticMeshInstance->ActivateObject(this);
					}

					DelayActivateObject.OwningActor = nullptr;
					DelayActivateObject.ScriptComponent = nullptr;
					DelayActivateObject.StaticMeshInstance = nullptr;
				}
			}
		}
	}
}

=======
// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

>>>>>>> 3f4b62b2900fe2b404f68c2ba0b2660ccffac8cb
// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
<<<<<<< HEAD

	UE_LOG(LogTemp, Warning, TEXT("Input set up!"));

	PlayerInputComponent->BindAction(FName("MouseClickLeft"), EInputEvent::IE_Pressed, this, &AMainCharacter::MouseLeftClick);
	PlayerInputComponent->BindAction(FName("MouseClickRight"), EInputEvent::IE_Released, this, &AMainCharacter::MouseRightClick);
}

float AMainCharacter::GetDistanceBetweenActors(AActor* Actor1, AActor* Actor2)
{
	return 0.0f;
}

// Inventory Functions
bool AMainCharacter::AddItemToInventory(InventoryItem* Item)
{
	return false;
}

void AMainCharacter::SwapInventoryElements(int32 index1, int32 index2)
{

}

void AMainCharacter::DropItem(int32 SlotIndex, FVector2D WorldLocation)
{

}

void AMainCharacter::DiscardItem(int32 SlotNum)
{

}
void AMainCharacter::DiscardItem(InventoryItem* item)
{

}

void AMainCharacter::ChangeCameraView(FVector Vector)
{
	FRotator rotation = FRotator::MakeFromEuler(Vector);
	CameraBoom->SetRelativeRotation(rotation.Quaternion());
}

// Blueprint Callable Functions!
FString AMainCharacter::GetInventoryTextureAt(int32 SlotNum)
{
	return "";
}

int32 AMainCharacter::GetInventorySize()
{
	return 0;
}

void AMainCharacter::SetCanRayCast(bool val)
{

}

void AMainCharacter::MouseLeftClick()
{
	if (!CanClickRayCast)
		return;

	FHitResult Hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		if (GameMode->IsInteractble(HitActor))
		{
			InteractableObject* Obj = GameMode->GetInteractableObject(HitActor);

			DelayActivateObject.OwningActor = Obj->OwningActor;
			DelayActivateObject.ScriptComponent = Obj->ScriptComponent;
			DelayActivateObject.StaticMeshInstance = Obj->StaticMeshInstance;

			MoveTo = Hit.ImpactPoint;
			DelayActivate = true;
			//MouseMove = true;
		}

		if (HitActor->IsA(AClimbableObject::StaticClass()))
		{
			FVector PathStart = GetActorLocation();

			FPathFindingQuery NavParams;
			NavParams.EndLocation = Hit.ImpactPoint;
			NavParams.StartLocation = GetActorLocation();
			ANavigationData* navData = GetWorld()->GetNavigationSystem()->MainNavData;
			NavParams.QueryFilter = UNavigationQueryFilter::GetQueryFilter<ANavigationData>(*navData);
			NavParams.NavData = navData;

			FPathFindingResult NavPath = NavSys->FindPathSync(NavParams);


			// We should check if we can climb the object here!
			UCapsuleComponent* OurCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
			float CapsuleHeight = OurCapsule->GetScaledCapsuleHalfHeight();

			if (NavPath.IsSuccessful())
			{
				if (NavPath.IsPartial())
				{
					if (!HitActor->IsA(AClimbableObject::StaticClass()))
					{
						FVector OurLocation = GetActorLocation();
						FVector TeleportLocation = Hit.ImpactPoint;
						TeleportLocation += FVector::UpVector*OurCapsule->GetScaledCapsuleHalfHeight();

						FVector Direction = TeleportLocation - OurLocation;

						float Zabs = FMath::Abs(Direction.Z);
						float Z = Direction.Z;

						DelayClimb = true;
						ClimbTo = TeleportLocation;
					}
					else {
						FVector Origin;
						FVector Extents;

						HitActor->GetActorBounds(false, Origin, Extents);

						FVector OurLocation = GetActorLocation();
						FVector TeleportLocation = HitActor->GetActorLocation();
						TeleportLocation += FVector::UpVector*Extents.Z;
						TeleportLocation += FVector::UpVector*CapsuleHeight;

						FVector Direction = OurLocation - TeleportLocation;
						Direction.Z = 0;
						Direction.Normalize();

						TeleportLocation += Direction*Extents.Z*0.7;

						DelayClimb = true;
						ClimbTo = TeleportLocation;
					}
				}
			}
			
		}

		MoveTo = Hit.ImpactPoint;
		MouseMove = true;
	}
}

void AMainCharacter::MouseRightClick()
{

=======
>>>>>>> 3f4b62b2900fe2b404f68c2ba0b2660ccffac8cb
}