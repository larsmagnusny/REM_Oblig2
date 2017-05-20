// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"
#include "InventoryItemObject.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> DialogueBlueprintClass(TEXT("WidgetBlueprint'/Game/UI/DialogueMenu.DialogueMenu_C'"));

	if (DialogueBlueprintClass.Succeeded())
	{
		DialogueWidgetClassTemplate = DialogueBlueprintClass.Class;
	}


	// Initialize the player inventory:
	PlayerInventory = new Inventory(5);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Load in the mesh if it has not been set...
	if (!SkeletalMeshComponent)
	{
		SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshLoader(TEXT("SkeletalMesh'/Game/Meshes/MainCharacter/CharacterMesh.CharacterMesh'"));
		if (SkeletalMeshLoader.Succeeded())
		{
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshLoader.Object);
			SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
			SkeletalMeshComponent->SetRelativeRotation(FRotator(0, -90, 0));

			if (!AnimClass)
			{
				SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

				static ConstructorHelpers::FObjectFinder<UClass> AnimationBP(TEXT("Class'/Game/Meshes/MainCharacter/AnimationBlueprint.AnimationBlueprint_C'"));

				if (AnimationBP.Succeeded())
				{
					SkeletalMeshComponent->SetAnimInstanceClass(AnimationBP.Object);
				}
			}

			static ConstructorHelpers::FObjectFinder<UMaterial> Mat1(TEXT("Material'/Game/Materials/Camo_Mat.Camo_Mat'"));
			static ConstructorHelpers::FObjectFinder<UMaterial> Mat2(TEXT("Material'/Game/Meshes/MainCharacter/lambert2.lambert2'"));

			if (Mat1.Succeeded() && Mat2.Succeeded())
			{
				StandardMaterial = Mat2.Object;
				CamoMaterial = Mat1.Object;
				SkeletalMeshComponent->SetMaterial(0, Mat2.Object);
			}
		}
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> RadioMeshLoader(TEXT("SkeletalMesh'/Game/Meshes/Radio/RadioRigged.RadioRigged'"));

	if (RadioMeshLoader.Succeeded())
	{
		RadioMesh = RadioMeshLoader.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> EyeMeshLoader(TEXT("StaticMesh'/Game/Meshes/MainCharacter/EYE.EYE'"));

	if (EyeMeshLoader.Succeeded())
	{
		EyeMesh = EyeMeshLoader.Object;
	}

	RadioComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RadioComponent"), true);
	EyeComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Eye1"), true);
	EyeComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Eye2"), true);

	MeshHolderComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshHolder"), true);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);
	

	EyeComponent1->AttachToComponent(SkeletalMeshComponent, Rules, FName("EyeSocket1"));
	EyeComponent2->AttachToComponent(SkeletalMeshComponent, Rules, FName("EyeSocket2"));
	RadioComponent->AttachToComponent(SkeletalMeshComponent, Rules, FName("Hold"));
	MeshHolderComponent->AttachToComponent(SkeletalMeshComponent, Rules, FName("ItemHold"));

	EyeComponent1->RegisterComponent();
	EyeComponent2->RegisterComponent();
	RadioComponent->RegisterComponent();
	MeshHolderComponent->RegisterComponent();

	EyeComponent1->SetMobility(EComponentMobility::Movable);
	EyeComponent2->SetMobility(EComponentMobility::Movable);
	MeshHolderComponent->SetMobility(EComponentMobility::Movable);
	MeshHolderComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	if(RadioMesh)
		RadioComponent->SetSkeletalMesh(RadioMesh);

	if (EyeMesh)
	{
		EyeComponent1->SetStaticMesh(EyeMesh);
		EyeComponent2->SetStaticMesh(EyeMesh);
	}
	
	RadioComponent->SetAllBodiesSimulatePhysics(true);
	

	// Set the viewtarget of this camera to look at the Main Character
	GetWorld()->GetFirstPlayerController()->SetViewTarget(this);

	// Setup inputs for the character ( mouse for the moment )
	SetupPlayerInputComponent(GetWorld()->GetFirstPlayerController()->InputComponent);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	if (MainCamera)
	{
		UCameraComponent* CameraComp = Cast<UCameraComponent>(MainCamera->GetComponentByClass(UCameraComponent::StaticClass()));
		ChangeCameraView(MainCamera);
		GameMode->SetMainCamera(CameraComp);
	}

	// Make the gamemode aware that we exist:
	GameMode->SetMainCharacter(this);

	// Hent NavigasjonsSystemet
	NavSys = GetWorld()->GetNavigationSystem();

	// Hent en peker til Hudden
	OurHud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Lag en Widget for Dialog
	if (DialogueWidgetClassTemplate)
	{
		DialogueWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetClassTemplate);
	}

	// Add til viewport
	if (DialogueWidget)
	{
		DialogueWidget->AddToViewport(4);
	}

	// Set den til usynelig
	SetDialogueChoiceInvisible();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Hvis spilleren ikke kan klikke på skjermen kan vi heller ikke bevege oss
	if (OurHud)
	{
		if (!OurHud->canPlayerClick)
			return;
		if (OurHud->DialogueMenuOpen)
			return;
	}

	// Raycast under the mouse so we can highlight the objects
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	FHitResult Hit;

	if (PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, Hit))
	{
		if (GameMode->IsInteractble(Hit.GetActor()))
		{
			// Get a pointer to the actor and MeshComponent
			AStaticMeshActor* OurActor = Cast<AStaticMeshActor>(Hit.GetActor());
			UInteractableComponent* InteractableComponent = GameMode->GetInteractor(Hit.GetActor());
			AInteractableObject* InteractableObj = GameMode->GetStaticMeshInteractor(Hit.GetActor());

			UStaticMeshComponent* MeshComponent = nullptr;
			USkeletalMeshComponent* TSkeletalMeshComponent = nullptr;

			if (InteractableComponent)
			{
				if (InteractableComponent->CanRightClick)
					OurHud->HintSnapToActor = Hit.GetActor();
				else
					OurHud->HintSnapToActor = nullptr;
			}
			else
			{
				OurHud->HintSnapToActor = nullptr;
			}

			if (!InteractableComponent && InteractableObj)
			{
				MeshComponent = InteractableObj->GetStaticMeshComponent();
			}
			if (!InteractableObj && InteractableComponent)
			{
				MeshComponent = InteractableComponent->GetStaticMeshComponent();
				TSkeletalMeshComponent = InteractableComponent->GetSkeletalMeshComponent();
			}

			if (TSkeletalMeshComponent)
				MeshComponent = nullptr;

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

			OurHud->HintSnapToActor = nullptr;
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

		OurHud->HintSnapToActor = nullptr;
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
			if (DelayActivate)
				ActivatePosition = GetActorLocation();
		}

		if (NavSys && (Distance > 25.0f))
		{
			NavSys->SimpleMoveToLocation(Controller, MoveTo);
		}
		else
		{
			MouseMove = false;

			float const ActivateDist = FVector2D::Distance(FVector2D(ActivatePosition), FVector2D(GetActorLocation()));

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

			if (DelayRunF)
			{
				if (DelayObject)
				{
					InventoryItem* Item = nullptr;

					if (DelaySlot != -1)
					{
						Item = PlayerInventory->GetItem(DelaySlot);

						if (BeginInteract && isInteracting)
						{
							MeshToShowWhenInteract = GameMode->MeshesAndTextures->GetStaticMeshByItemID(Item->ItemID);

							TArray<UMaterial*> Mats = GameMode->MeshesAndTextures->GetMaterialsByItemID(Item->ItemID);

							if (MeshHolderComponent->GetStaticMesh() != MeshToShowWhenInteract)
							{
								// SetMesh
								MeshHolderComponent->SetStaticMesh(MeshToShowWhenInteract);

								for (int i = 0; i < Mats.Num(); i++)
									MeshHolderComponent->SetMaterial(i, Mats[i]);
							}

							MouseMove = true;

							return;
						}
						else if (isInteracting && !BeginInteract)
						{
							MouseMove = true;
							return;
						}

						if (!isInteracting && !BeginInteract && MeshToShowWhenInteract == nullptr)
						{
							isInteracting = true;

							MouseMove = true;
							return;
						}
						else {
							MeshToShowWhenInteract = nullptr;
							isInteracting = false;
							BeginInteract = false;
							MeshHolderComponent->SetStaticMesh(MeshToShowWhenInteract);
						}
					}

					UE_LOG(LogTemp, Error, TEXT("Running Switch!"));
					DelayRunF = false;
					switch (DelayAction)
					{
					case ActionType::INTERACT_ACTIVATE:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->ActivateObject(this);
						}
						if (DelayObject->StaticMeshInstance)
						{
							DelayObject->StaticMeshInstance->ActivateObject(this);
						}
						break;
					case ActionType::INTERACT_EXAMINE:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->ExamineObject(this);
						}
						else {
							UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
						}
						break;
					case ActionType::INTERACT_OPENINVENTORY:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->OpenInventory(this);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
						}
						break;
					case ActionType::INTERACT_PICKUP:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->PickupObject(this);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
						}
						break;
					case ActionType::INTERACT_DIALOGUE:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->ActivateDialogue(this);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
						}
						break;
					case ActionType::INTERACT_ITEM:
						if (DelayObject->ScriptComponent)
						{
							DelayObject->ScriptComponent->ItemInteract(DelaySlot);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Action not implementet for this type of object, fix menu of item."));
						}
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("Action not implemented!"));
					}

					DelaySlot = -1;
				}
			}

			if (DelayActivate)
			{
				if (ActivateDist > 50.f)
					return;

				UE_LOG(LogTemp, Error, TEXT("Inside..."));

				if (DelayActivateObject.OwningActor->IsA(AInventoryItemObject::StaticClass()))
				{
					if (PutInHand)
					{
						AInventoryItemObject* Object = Cast<AInventoryItemObject>(DelayActivateObject.OwningActor);
						UStaticMeshComponent* MeshComponentPickup = Object->GetStaticMeshComponent();
						MeshToShowWhenInteract = MeshComponentPickup->StaticMesh;
						
						TArray<UMaterialInterface*> Mats = MeshComponentPickup->GetMaterials();
						
						if (MeshHolderComponent->GetStaticMesh() != MeshToShowWhenInteract)
						{
							// SetMesh
							MeshHolderComponent->SetStaticMesh(MeshToShowWhenInteract);
							
							for (int i = 0; i < Mats.Num(); i++)
								MeshHolderComponent->SetMaterial(i, Mats[i]);

							Cast<AInventoryItemObject>(DelayActivateObject.OwningActor)->GetStaticMeshComponent()->SetVisibility(false, true);
						}
						MouseMove = true;

						return;
					}
					else if (isInteracting && !PutInHand)
					{
						MouseMove = true;
						return;
					}

					if (!isInteracting && !PutInHand && MeshToShowWhenInteract == nullptr)
					{
						isInteracting = true;

						MouseMove = true;
						return;
					}
					else {
						MeshToShowWhenInteract = nullptr;
						MeshHolderComponent->SetStaticMesh(MeshToShowWhenInteract);
					}
				}
				else {
					isInteracting = true;
				}

				UE_LOG(LogTemp, Error, TEXT("Outside..."));

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
	PlayerInputComponent->BindAction(FName("Pause"), EInputEvent::IE_Pressed, this, &AMainCharacter::TogglePauseMenu);
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

void AMainCharacter::ChangeCameraView(AActor* Camera)
{
	UCameraComponent* CameraComponent = Cast<UCameraComponent>(Camera->GetComponentByClass(UCameraComponent::StaticClass()));

	UE_LOG(LogTemp, Warning, TEXT("Switched Camera!"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetViewTarget(Camera);
}

// Blueprint Callable Functions!
UTexture2D* AMainCharacter::GetInventoryTextureAt(int32 SlotNum)
{
	return PlayerInventory->GetTextureReference(SlotNum);
}

int32 AMainCharacter::GetInventorySize()
{
	return PlayerInventory->GetSize();
}

void AMainCharacter::SetCanRayCast(bool val)
{

}

void AMainCharacter::ReloadInventory(FBufferArchive & BinaryData, MeshAndTextureLoader * ResourceLoader)
{
	PlayerInventory->LoadInventoryFromPersistent(BinaryData, ResourceLoader);
}

void AMainCharacter::SaveInventory(FBufferArchive & BinaryData)
{
	PlayerInventory->SaveInventoryToPersistent(BinaryData);
}

void AMainCharacter::MouseLeftClick()
{
	if (MeshToShowWhenInteract)
		return;

	DelayActivate = false;
	DelayRunF = false;
	DelayClimb = false;
	isInteracting = false;
	PutInHand = false;
	BeginInteract = false;

	if (OurHud)
	{
		if (OurHud->DialogueMenuOpen)
		{
			// Send to our dialogue menu...
			AnywhereClicked = true;
			return;
		}

		if (!OurHud->canPlayerClick)
			return;
	}

	if (SpaceBarDown)
		return;
	if (!CanClickRayCast)
		return;

	FHitResult Hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, Hit);

	AActor* HitActor = Hit.GetActor();

	// We are in a puzzleGameMode...
	if (IsInPuzzleGameMode)
	{
		if (HitActor)
		{
			if (GameMode->IsInteractble(HitActor))
			{
				InteractableObject* Obj = GameMode->GetInteractableObject(HitActor);

				if (Obj->ScriptComponent)
				{
					Obj->ScriptComponent->ActivateObject(this);
				}
				if (Obj->StaticMeshInstance)
				{
					Obj->StaticMeshInstance->ActivateObject(this);
				}
			}
			else {
				UInteractableComponent* Component = Cast<UInteractableComponent>(HitActor->GetComponentByClass(UInteractableComponent::StaticClass()));

				if (Component)
				{
					Component->ActivateObject(this);
				}
			}
		}
		return;
	}


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
	if (OurHud)
	{
		if (MeshToShowWhenInteract)
			return;

		DelayClimb = false;
		DelayRunF = false;
		DelayActivate = false;
		isInteracting = false;
		PutInHand = false;
		BeginInteract = false;

		if (SpaceBarDown)
			return;

		if (!CanClickRayCast)
			return;

		if (!OurHud->canPlayerClick)
			return;

		if (OurHud->DialogueMenuOpen)
			return;

		FHitResult Hit;
		GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, Hit);

		AActor* HitActor = Hit.GetActor();

		if (IsInPuzzleGameMode)
		{
			if (HitActor)
			{
				if (GameMode->IsInteractble(HitActor))
				{
					InteractableObject* Obj = GameMode->GetInteractableObject(HitActor);

					if (Obj->ScriptComponent)
					{
						Obj->ScriptComponent->ActivateObject(this);
					}
					if (Obj->StaticMeshInstance)
					{
						Obj->StaticMeshInstance->ActivateObject(this);
					}
				}
				else {
					UInteractableComponent* Component = Cast<UInteractableComponent>(HitActor->GetComponentByClass(UInteractableComponent::StaticClass()));

					if (Component)
					{
						Component->ActivateObject(this);
					}
				}
			}
			return;
		}

		if (HitActor)
		{
			if (GameMode->IsInteractble(HitActor))
			{
				UInteractableComponent* Component = GameMode->GetInteractor(HitActor);

				if (!Component->CanRightClick)
					return;

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

void AMainCharacter::TogglePauseMenu()
{
	if (OurHud)
	{
		OurHud->TogglePauseMenuVisibility();
	}
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

void AMainCharacter::ClearDialogueOptions()
{
	CurrentDialogueOptions.Empty();
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