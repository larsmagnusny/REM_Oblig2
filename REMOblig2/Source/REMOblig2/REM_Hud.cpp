// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"
#include "REM_GameMode.h"
#include "REM_Hud.h"

AREM_Hud::AREM_Hud()
{
	
}

void AREM_Hud::BeginPlay()
{
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;
}

void AREM_Hud::DrawHUD()
{
	Super::DrawHUD();

	if (!RightClickMenu)
	{
		return;
	}
	if (MenuSnapToActor)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), MenuSnapToActor->GetActorLocation(), ScreenPos, false);
		
		ScreenPos += FVector2D(-112, -122)*ViewportScale;

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

void AREM_Hud::AddInteractionWidget(AActor* OwnerObject, UUserWidget* Widget, UInteractableComponent* Component, AInteractableObject* Object)
{
	InteractionWidget InterWidget;
	InterWidget.Owner = OwnerObject;
	InterWidget.MenuWidget = Widget;
	InterWidget.ParentComponent = Component;
	InterWidget.ParentObject = Object;
	SubMenues.Add(InterWidget);
}

InteractionWidget* AREM_Hud::GetParentInteractorI(UUserWidget* Widget)
{
	for (int32 i = 0; i < SubMenues.Num(); i++)
	{
		if (SubMenues[i].MenuWidget == Widget)
			return &SubMenues[i];
	}

	return nullptr;
}

InteractionWidget* AREM_Hud::GetParentInteractorI(AActor* Owner)
{
	for (int32 i = 0; i < SubMenues.Num(); i++)
	{
		if (SubMenues[i].Owner == Owner)
			return &SubMenues[i];
	}

	return nullptr;
}

UInteractableComponent* AREM_Hud::GetParentInteractor(UUserWidget* Widget)
{
	for (int32 i = 0; i < SubMenues.Num(); i++)
	{
		if (SubMenues[i].MenuWidget == Widget && SubMenues[i].MenuWidget)
			return SubMenues[i].ParentComponent;
	}

	return nullptr;
}

UUserWidget* AREM_Hud::HUDCreateWidget(UClass* Template)
{
	return CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), Template);
}