// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameMode.h"
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
	RightClickMenu->SetVisibility(ESlateVisibility::Hidden);
}

void AREM_Hud::DrawHUD()
{
	Super::DrawHUD();

	if (ShowRightClickMenu)
	{
		RightClickMenu->SetVisibility(ESlateVisibility::Visible);
	}

	if (MenuSnapToActor)
	{
		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), MenuSnapToActor->GetActorLocation(), ScreenPos, false);
		
		ScreenPos += FVector2D(-64, -64);

		RightClickMenu->SetPositionInViewport(ScreenPos, true);
	}
}

void AREM_Hud::CallActivate(ActionType Action)
{
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	InteractableObject* Obj = GameMode->GetInteractableObject(MenuSnapToActor);
	if (Obj)
	{
		switch (Action)
		{
		case ActionType::INTERACT_ACTIVATE:
			if (Obj->ScriptComponent)
				Obj->ScriptComponent->ActivateObject(GameMode->GetMainCharacter());
			if (Obj->StaticMeshInstance)
				Obj->StaticMeshInstance->ActivateObject(GameMode->GetMainCharacter());
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Action not implemented!"));
		}
	}
}