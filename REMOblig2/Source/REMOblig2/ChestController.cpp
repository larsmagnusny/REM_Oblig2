// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "ChestController.h"


// Sets default values for this component's properties
UChestController::UChestController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Add what buttons this object has on its menu system...
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::OPEN]);
	Actions.Add(ActionType::INTERACT_OPENINVENTORY);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::PICKUP]);
	Actions.Add(ActionType::INTERACT_PICKUP);

	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::USE]);
	Actions.Add(ActionType::INTERACT_ACTIVATE);

	
}


// Called when the game starts
void UChestController::BeginPlay()
{
	Super::BeginPlay();

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

	GameMode->AddInteractableObject(GetOwner(), this, nullptr);
}


// Called every frame
void UChestController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}

void UChestController::ActivateObject(AActor* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Should open chest now!"));
	if (FollowWhenActivate)
	{
		PlayerToFollow = Player;
		FollowingPlayer = true;
	}
	else
	{
		isOpen = !isOpen;
	}
}