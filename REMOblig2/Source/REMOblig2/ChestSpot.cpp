// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "ChestSpot.h"

UChestSpot::UChestSpot()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Add what buttons this object has on its menu system...
	ObjectSpecificMenuButtons.Add(MenuButtons[ButtonTypes::EXAMINE]);
	Actions.Add(ActionType::INTERACT_EXAMINE);
}

void UChestSpot::BeginPlay()
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

	GameMode->AddInteractableObject(GetOwner(), this);
}

void UChestSpot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UChestSpot::ActivateObject(AActor* Player)
{
	print("There's an x marking the spot. I wonder if i place something here something will happen?");
}

FVector UChestSpot::GetActivatePosition(AActor* Player)
{
	return GetOwner()->GetActorLocation();
}