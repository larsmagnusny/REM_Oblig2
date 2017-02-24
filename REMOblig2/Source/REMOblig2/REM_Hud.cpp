// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_Hud.h"

AREM_Hud::AREM_Hud()
{
	// Load menues we are going to use
	static ConstructorHelpers::FClassFinder<UUserWidget> RightClickMenuClass(TEXT("WidgetBlueprint'/Game/Blueprints/Menues/InteractMenu.InteractMenu_C'"));
	
	if (RightClickMenuClass.Succeeded())
	{
		RightClickMenuClassTemplate = RightClickMenuClass.Class;
	}
}

void AREM_Hud::BeginPlay()
{
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;

	if (RightClickMenuClassTemplate)
	{
		RightClickMenu = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), RightClickMenuClassTemplate);
	}

	RightClickMenu->AddToViewport();
}

void AREM_Hud::DrawHUD()
{
	Super::DrawHUD();

	if (MenuSnapToActor)
	{
		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), MenuSnapToActor->GetActorLocation(), ScreenPos, false);
		RightClickMenu->SetPositionInViewport(ScreenPos, true);
	}
}