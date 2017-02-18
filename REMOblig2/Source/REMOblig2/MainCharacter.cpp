// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	GetWorld()->GetFirstPlayerController()->SetViewTarget(this);
}

// Called every frame
void AMainCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

}

void AMainCharacter::MouseRightClick()
{

}