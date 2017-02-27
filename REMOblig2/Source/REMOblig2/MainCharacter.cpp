// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> DialogueBlueprintClass(TEXT("WidgetBlueprint'/Game/Blueprints/Menues/DialogueMenu.DialogueMenu_C'"));

	if (DialogueBlueprintClass.Succeeded())
	{
		DialogueWidgetClassTemplate = DialogueBlueprintClass.Class;
	}


	// Initialize the player inventory:
	PlayerInventory = new Inventory(5);

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

			static ConstructorHelpers::FObjectFinder<UMaterial> Mat1(TEXT("Material'/Game/Materials/Camo_Mat.Camo_Mat'"));
			static ConstructorHelpers::FObjectFinder<UMaterial> Mat2(TEXT("Material'/Game/Meshes/Mannequin/Character/Materials/M_UE4Man_Body.M_UE4Man_Body'"));

			if (Mat1.Succeeded() && Mat2.Succeeded())
			{
				StandardMaterial = Mat2.Object;
				CamoMaterial = Mat1.Object;
				SkeletalMeshComponent->SetMaterial(0, Mat2.Object);
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

	OurHud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (DialogueWidgetClassTemplate)
	{
		DialogueWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetClassTemplate);
	}

	if (DialogueWidget)
	{
		DialogueWidget->AddToViewport();
	}

	SetDialogueChoiceInvisible();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OurHud)
	{
		if (!OurHud->canPlayerClick)
			return;
	}

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

	if (SpaceBarDown)
	{
		MouseMove = false;
		// 2D distance to target...
		float const Distance = FVector2D::Distance(FVector2D(MoveTo), FVector2D(GetActorLocation()));
		if (Distance == lastDistance)
			MoveTo = GetActorLocation();
		lastDistance = Distance;

		if (NavSys && (Distance > 25.0f))
		{
			Hiding = false;
			NavSys->SimpleMoveToLocation(Controller, MoveTo);
		}
		else {
			Hiding = true;
			
			SetActorRotation(HideNormal.ToOrientationQuat(), ETeleportType::TeleportPhysics);
			if (SkeletalMeshComponent)
			{
				// Set our material to the masked material!
				if (SkeletalMeshComponent->GetMaterial(0) != CamoMaterial)
				{
					SkeletalMeshComponent->SetMaterial(0, CamoMaterial);
				}
			}
		}
	}
	else if (!MouseMove)
	{
		NavSys->SimpleMoveToLocation(Controller, GetActorLocation());

		if (SkeletalMeshComponent)
		{
			// Set our material to the masked material!
			if (SkeletalMeshComponent->GetMaterial(0) != StandardMaterial)
			{
				SkeletalMeshComponent->SetMaterial(0, StandardMaterial);
			}
		}
	}
	

	// If the player clicked on the screen somewhere...
	if (MouseMove)
	{
		// 2D distance to target...
		float const Distance = FVector2D::Distance(FVector2D(MoveTo), FVector2D(GetActorLocation()));
		if (Distance == lastDistance)
			lastDistanceCounter++;
		else
			lastDistanceCounter = 0;

		lastDistance = Distance;

		if (lastDistanceCounter > 5)
		{
			MoveTo = GetActorLocation();
		}

		if (NavSys && (Distance > 25.0f))
		{
			NavSys->SimpleMoveToLocation(Controller, MoveTo);
		}
		else
		{
			MouseMove = false;

			float const ActivateDist = FVector2D::Distance(FVector2D(ActivatePosition), FVector2D(GetActorLocation()));

			if (ActivateDist > 30.f)
				return;

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
						DelayActivateObject.ScriptComponent->ActivateObject(this);
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

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("Input set up!"));

	PlayerInputComponent->BindAction(FName("MouseClickLeft"), EInputEvent::IE_Pressed, this, &AMainCharacter::MouseLeftClick);
	PlayerInputComponent->BindAction(FName("MouseClickRight"), EInputEvent::IE_Pressed, this, &AMainCharacter::MouseRightClick);
	PlayerInputComponent->BindAction(FName("SpaceBar"), EInputEvent::IE_Pressed, this, &AMainCharacter::SpaceBarPressed);
	PlayerInputComponent->BindAction(FName("SpaceBar"), EInputEvent::IE_Released, this, &AMainCharacter::SpaceBarReleased);
}

float AMainCharacter::GetDistanceBetweenActors(AActor* Actor1, AActor* Actor2)
{
	return 0.0f;
}

// Inventory Functions
bool AMainCharacter::AddItemToInventory(InventoryItem* Item)
{
	return PlayerInventory->AddItem(Item);
}

void AMainCharacter::SwapInventoryElements(int32 index1, int32 index2)
{

}

void AMainCharacter::DropItem(int32 SlotIndex, FVector2D WorldLocation)
{
	FVector Position;
	FVector Rotation;
	FVector Scale;

	FVector CharacterPosition = GetActorLocation();

	FVector2D dir = WorldLocation - FVector2D(CharacterPosition);

	dir.Normalize();

	Position = CharacterPosition + 50 * FVector(dir, 0.5);

	Rotation = FVector(0, 0, 0);
	Scale = FVector(1, 1, 1);

	GameMode->PutObjectInWorld(PlayerInventory->GetItem(SlotIndex), Position, Rotation, Scale);
	PlayerInventory->DiscardItem(SlotIndex);
}

void AMainCharacter::DiscardItem(int32 SlotNum)
{
	PlayerInventory->DiscardItem(SlotNum);
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
UTexture2D* AMainCharacter::GetInventoryTextureAt(int32 SlotNum)
{
	return PlayerInventory->GetTextureReference(SlotNum);
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
	if (OurHud)
	{
		if (!OurHud->canPlayerClick)
			return;
	}

	if (SpaceBarDown)
		return;
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

			if (Obj->StaticMeshInstance)
			{
				MoveTo = Hit.ImpactPoint;
			}
			if (Obj->ScriptComponent)
			{
				MoveTo = Obj->ScriptComponent->GetActivatePosition(this);
			}
			ActivatePosition = MoveTo;
			DelayActivate = true;
			//MouseMove = true;
		}
		else
		{
			MoveTo = Hit.ImpactPoint;
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

		//MoveTo = Hit.ImpactPoint;
		MouseMove = true;
	}
}

void AMainCharacter::MouseRightClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Clicked Mouse!"));
	if (OurHud)
	{
		if (SpaceBarDown)
			return;
		if (!CanClickRayCast)
			return;

		if (!OurHud->canPlayerClick)
			return;

		FHitResult Hit;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit);

		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (GameMode->IsInteractble(HitActor))
			{
				InteractionWidget* IWidget = OurHud->GetParentInteractorI(HitActor);
				if (IWidget)
				{
					if (IWidget->ParentComponent)
					{
						if (OurHud->MenuSnapToActor && OurHud->MenuSnapToActor != HitActor)
						{
							InteractionWidget* OtherIWidget = OurHud->GetParentInteractorI(OurHud->MenuSnapToActor);

							if (OtherIWidget)
							{
								if (OtherIWidget->ParentComponent)
								{
									if (OtherIWidget->ParentComponent->ShowRightClickMenu)
									{
										OtherIWidget->ParentComponent->ShowRightClickMenu = false;
										OtherIWidget->ParentComponent->ShowAnimationBackwards = true;
										OurHud->RightClickMenu = nullptr;
										OurHud->MenuSnapToActor = nullptr;
									}
								}
							}
						}

						IWidget->ParentComponent->ShowRightClickMenu = true;
						IWidget->ParentComponent->ShowAnimation = true;
						OurHud->RightClickMenu = IWidget->MenuWidget;
						OurHud->MenuSnapToActor = HitActor;
					}
				}
			}
			else if(OurHud->MenuSnapToActor)
			{
				InteractionWidget* IWidget = OurHud->GetParentInteractorI(OurHud->MenuSnapToActor);

				if (IWidget)
				{
					if (IWidget->ParentComponent)
					{
						if (IWidget->ParentComponent->ShowRightClickMenu)
						{
							IWidget->ParentComponent->ShowRightClickMenu = false;
							IWidget->ParentComponent->ShowAnimationBackwards = true;
							OurHud->RightClickMenu = nullptr;
							OurHud->MenuSnapToActor = nullptr;
						}
					}
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("Could not find interactor!"));
				}
			}
		}
		else if (OurHud->MenuSnapToActor)
		{
			InteractionWidget* IWidget = OurHud->GetParentInteractorI(OurHud->MenuSnapToActor);

			if (IWidget)
			{
				if (IWidget->ParentComponent)
				{
					if (IWidget->ParentComponent->ShowRightClickMenu)
					{
						IWidget->ParentComponent->ShowRightClickMenu = false;
						IWidget->ParentComponent->ShowAnimationBackwards = true;
						OurHud->RightClickMenu = nullptr;
						OurHud->MenuSnapToActor = nullptr;
					}
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Could not find interactor!"));
			}
		}
	}
}

void AMainCharacter::SpaceBarPressed()
{
	SpaceBarDown = true;

	// RayCast in four directions and determine which direction is closest, then move to that location and activate cloaking mechanism...
	FVector Start[4];

	for (int i = 0; i < 4; i++)
	{
		Start[i] = GetActorLocation();
	}

	FVector Direction[4];
	Direction[0] = FVector::ForwardVector;		// Forward
	Direction[1] = -FVector::ForwardVector;	    // Back
	Direction[2] = FVector::RightVector;		// Right
	Direction[3] = -FVector::RightVector;		// Left

	FHitResult Rays[4];

	GameMode->RayCastArray(Rays, Start, Direction, 10000.f, 4, this);

	for (int i = 0; i < 4; i++)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *Rays[i].ImpactPoint.ToString());
	}

	float Distances[4];

	for (int i = 0; i < 4; i++)
	{
		Distances[i] = FVector::Dist(GetActorLocation(), Rays[i].ImpactPoint);
	}

	float min = INT_MAX;
	int smallest_index = 0;

	for (int i = 0; i < 4; i++)
	{
		if (Distances[i] < min)
		{
			min = Distances[i];
			smallest_index = i;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Should move to: %s"), *Rays[smallest_index].ImpactPoint.ToString());

	MoveTo = Rays[smallest_index].ImpactPoint;
	HideNormal = Rays[smallest_index].ImpactNormal;
}

void AMainCharacter::SpaceBarReleased()
{
	SpaceBarDown = false;
}

// For setting the dialogue options the player has when interacting with an object
void AMainCharacter::SetDialogueOptions(TArray<FString> Options, UInteractableComponent* Caller)
{
	TalkingTo = Caller;

	CurrentDialogueOptions.Empty();

	CurrentDialogueOptions = Options;

	ShouldReloadDialogues = true;
}

FString AMainCharacter::GetDialogueOption(int i)
{
	if (i <= CurrentDialogueOptions.Num() - 1)
	{
		return CurrentDialogueOptions[i];
	}
	else
	{
		return "";
	}
}
UInteractableComponent* AMainCharacter::GetTalkingTo()
{
	return TalkingTo;
}

void AMainCharacter::SetDialogueChoiceVisible()
{
	if (DialogueWidget)
	{
		DialogueWidget->SetVisibility(ESlateVisibility::Visible);
	}

	//SetDialogueChoiceInvisible();
}

void AMainCharacter::SetDialogueChoiceInvisible()
{
	if (DialogueWidget)
	{
		DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

InventoryItem* AMainCharacter::GetItemByID(ItemIDs ID)
{
	return PlayerInventory->GetItemById(ID);
}

InventoryItem* AMainCharacter::GetItemBySlot(int32 SlotNum)
{
	return PlayerInventory->GetItem(SlotNum);
}