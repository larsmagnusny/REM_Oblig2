// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "LightSwitchComponent.h"
#include "MainCharacter.h"

ULightSwitchComponent::ULightSwitchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CanRightClick = false;
}

void ULightSwitchComponent::BeginPlay()
{
	Super::BeginPlay();

	// Legg til meny-valg for "Right-Click" Meny
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
	Hud = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// SubMenuWidgetClass definert i UInteractableComponent
	if (SubMenuWidgetClassTemplate)
	{
		SubMenuWidget = Hud->HUDCreateWidget(SubMenuWidgetClassTemplate);

		if (SubMenuWidget)
		{
			Hud->AddInteractionWidget(GetOwner(), SubMenuWidget, this);
			SubMenuWidget->AddToViewport(11);
		}
	}

	// Fortell gamemode at denne er interactable

	GameMode->AddInteractableObject(GetOwner(), this);
}

void ULightSwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (On != LastState)
	{
		for (AActor* Light : Lights)
		{
			if (Light->IsA(APointLight::StaticClass()))
			{
				UPointLightComponent* Component = Cast<UPointLightComponent>(Light->GetComponentByClass(UPointLightComponent::StaticClass()));

				if (On)
				{
					Component->SetIntensity(OnIntensity);
				}
				if(!On)
				{
					Component->SetIntensity(OffIntensity);
				}
			}
		}

		LastState = On;
	}
}

void ULightSwitchComponent::ActivateObject(AActor * Player)
{
	On = !On;
}

void ULightSwitchComponent::ExamineObject(AActor * Player)
{
	TArray<FString> Conversation;

	Conversation.Add("It's a light switch, it turns the light on and off.");

	Cast<AMainCharacter>(Player)->Conversation = Conversation;
	Cast<AMainCharacter>(Player)->ShouldShowConversation = true;
	Cast<AMainCharacter>(Player)->SetDialogueChoiceVisible();
}

FVector ULightSwitchComponent::GetActivatePosition(AActor * Player)
{
	UE_LOG(LogTemp, Error, TEXT("Ran overridden function"));
	FVector PlayerPosition = Player->GetActorLocation();
	FVector OurPosition = GetOwner()->GetActorLocation();
	FVector Forward = GetOwner()->GetActorForwardVector();
	FVector FinalPosition = OurPosition + Forward*15.f;
	FinalPosition.Z = PlayerPosition.Z;
	return FinalPosition;
}

FBufferArchive ULightSwitchComponent::GetSaveData()
{
	FBufferArchive BinaryData;

	BinaryData << On;

	return BinaryData;
}

void ULightSwitchComponent::LoadSaveData(FMemoryReader & Ar)
{
	Ar << On;

	LastState = !On;
}
