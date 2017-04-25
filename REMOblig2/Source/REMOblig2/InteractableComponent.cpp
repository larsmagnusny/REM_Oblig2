// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "InteractableComponent.h"
#include "MainCharacter.h"
#include "REM_GameMode.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Last inn right click menyen
	ConstructorHelpers::FClassFinder<UUserWidget> SubMenuClass(TEXT("WidgetBlueprint'/Game/UI/InteractMenu.InteractMenu_C'"));

	if (SubMenuClass.Succeeded())
	{
		SubMenuWidgetClassTemplate = SubMenuClass.Class;
	}
	
	MenuButtons.Add(new UMenuIconsDef("Texture2D'/Game/Textures/ExamineButtonCircle.ExamineButtonCircle'",
									 "Texture2D'/Game/Textures/ExamineButtonCircleHover.ExamineButtonCircleHover'",
									 "Texture2D'/Game/Textures/ExamineButtonCircleClick.ExamineButtonCircleClick'"));

	MenuButtons.Add(new UMenuIconsDef("Texture2D'/Game/Textures/OpenButtonCircle.OpenButtonCircle'",
									 "Texture2D'/Game/Textures/OpenButtonCircleHover.OpenButtonCircleHover'",
									 "Texture2D'/Game/Textures/OpenButtonCircleClick.OpenButtonCircleClick'"));

	MenuButtons.Add(new UMenuIconsDef("Texture2D'/Game/Textures/PickupButtonCircle.PickupButtonCircle'",
									 "Texture2D'/Game/Textures/PickupButtonCircleHover.PickupButtonCircleHover'",
									 "Texture2D'/Game/Textures/PickupButtonCircleClick.PickupButtonCircleClick'"));

	MenuButtons.Add(new UMenuIconsDef("Texture2D'/Game/Textures/UseButtonCircle.UseButtonCircle'",
									 "Texture2D'/Game/Textures/UseButtonCircleHover.UseButtonCircleHover'",
									 "Texture2D'/Game/Textures/UseButtonCircleClick.UseButtonCircleClick'"));

	MenuButtons.Add(new UMenuIconsDef("Texture2D'/Game/Textures/DialogueCircle.DialogueCircle'",
									  "Texture2D'/Game/Textures/DialogueCircleHover.DialogueCircleHover'",
									  "Texture2D'/Game/Textures/DialogueCircleClick.DialogueCircleClick'"));
}

void UInteractableComponent::ActivateObject(AActor* Player)
{

}

void UInteractableComponent::ExamineObject(AActor* Player)
{

}

void UInteractableComponent::PickupObject(AActor* Player)
{

}

void UInteractableComponent::OpenInventory(AActor* Player)
{

}

void UInteractableComponent::ActivateDialogue(AActor* Player)
{

}

FBufferArchive UInteractableComponent::GetSaveData()
{
	FBufferArchive BinaryData;
	return BinaryData;
}

void UInteractableComponent::LoadSaveData(FMemoryReader &Ar)
{

}

void UInteractableComponent::ItemInteract(int32 SlotNum)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>((Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode())->GetMainCharacter()));
	TArray<FString> Conversation;
	Conversation.Add("Nothing Happenes..");

	MainCharacter->Conversation = Conversation;
	MainCharacter->ShouldShowConversation = true;
	MainCharacter->SetDialogueChoiceVisible();
}

UStaticMeshComponent* UInteractableComponent::GetStaticMeshComponent()
{
	return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

USkeletalMeshComponent* UInteractableComponent::GetSkeletalMeshComponent()
{
	return Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}

FVector UInteractableComponent::GetActivatePosition(AActor* Player)
{
	// Denne brukes til � fastsl� en posisjon som er rett foran ett objekt, s� karakteren ikke g�r inn i objektet
	FVector ActorLocation = GetOwner()->GetActorLocation();

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	bool StaticMesh = false;

	if (MeshComponent)
		StaticMesh = true;

	
	FVector Bounds = FVector::ZeroVector;
	
	if (StaticMesh)
		Bounds = MeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
	else
		Bounds = SkeletalMeshComponent->Bounds.BoxExtent;

	if (Bounds == FVector::ZeroVector)
		return GetOwner()->GetActorLocation();

	FVector ActivatePosition = ActorLocation + GetOwner()->GetActorForwardVector()*(Bounds.X + 20);

	return ActivatePosition;
}

int UInteractableComponent::GetNumMenuButtons()
{
	return ObjectSpecificMenuButtons.Num();
}

TArray<UTexture2D*> UInteractableComponent::GetMenuButtonTextures(int i)
{
	TArray<UTexture2D*> Textures;
	if (i <= ObjectSpecificMenuButtons.Num() - 1)
	{
		Textures.Add(ObjectSpecificMenuButtons[i]->Textures[0]);
		Textures.Add(ObjectSpecificMenuButtons[i]->Textures[1]);
		Textures.Add(ObjectSpecificMenuButtons[i]->Textures[2]);
	}

	return Textures;
}

ActionType UInteractableComponent::GetActionType(int i)
{
	if (i <= Actions.Num() - 1)
	{
		return Actions[i];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find it!"));
	}

	return ActionType::INTERACT_ACTIVATE;
}

void UInteractableComponent::DialogueOptionPressed(UUserWidget* Caller, int optionindex)
{

}