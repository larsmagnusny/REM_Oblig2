// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"
#include "MainCharacter.h"
#include "REM_Hud.h"

AREM_Hud::AREM_Hud()
{
	ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetLoader(TEXT("WidgetBlueprint'/Game/Blueprints/Menues/Inventory.Inventory_C'"));

	if (InventoryWidgetLoader.Succeeded())
	{
		InventoryWidgetClassTemplate = InventoryWidgetLoader.Class;
	}
}

void AREM_Hud::BeginPlay()
{
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;

	if (InventoryWidgetClassTemplate)
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InventoryWidgetClassTemplate);

		InventoryWidget->AddToViewport(0);
	}

	for (int32 i = 0; i < 5; i++)
	{
		FString name = "Slot";

		name += FString::FromInt(i);

		UUserWidget* Slot = Cast<UUserWidget>(InventoryWidget->GetWidgetFromName(FName(*name)));

		UImage* Image = nullptr;

		if (Slot)
		{
			Image = Cast<UImage>(Slot->GetWidgetFromName("Image0"));

			if (Image)
				Slots.Add(Image);
		}
	}

	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
}

void AREM_Hud::DrawHUD()
{
	Super::DrawHUD();
	AMainCharacter* MainCharacter = nullptr;
	if(GameMode)
		MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	if (MainCharacter)
	{
		for (int32 i = 0; i < Slots.Num(); i++)
		{
			UTexture2D* tex = MainCharacter->GetInventoryTextureAt(i);

			if (tex)
			{
				Slots[i]->SetBrushFromTexture(tex, true);
				Slots[i]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Slots[i]->SetBrush(FSlateBrush());
				Slots[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

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
			{
				Obj->ScriptComponent->ActivateObject(GameMode->GetMainCharacter());
			}
			if (Obj->StaticMeshInstance)
			{
				Obj->StaticMeshInstance->ActivateObject(GameMode->GetMainCharacter());
			}
			break;
		case ActionType::INTERACT_EXAMINE:
			if (Obj->ScriptComponent)
			{
				Obj->ScriptComponent->ExamineObject(GameMode->GetMainCharacter());
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
			}
			break;
		case ActionType::INTERACT_OPENINVENTORY:
			if (Obj->ScriptComponent)
			{
				Obj->ScriptComponent->OpenInventory(GameMode->GetMainCharacter());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
			}
			break;
		case ActionType::INTERACT_PICKUP:
			if (Obj->ScriptComponent)
			{
				Obj->ScriptComponent->PickupObject(GameMode->GetMainCharacter());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
			}
			break;
		case ActionType::INTERACT_DIALOGUE:
			if (Obj->ScriptComponent)
			{
				Obj->ScriptComponent->ActivateDialogue(GameMode->GetMainCharacter());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Action not implemented for this type of object, fix menu of item."));
			}
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Action not implemented!"));
		}
	}

	return;
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

void AREM_Hud::SwapElements(int32 index1, int32 index2)
{

}

void AREM_Hud::DropItem(int32 SlotIndex, FVector2D HitPoint)
{
	AMainCharacter* MainCharacter = nullptr;
	if (GameMode)
		MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	if (MainCharacter)
		MainCharacter->DropItem(SlotIndex, HitPoint);
}

bool AREM_Hud::IsActorInteractable(AActor* Actor)
{
	if (GameMode)
	{
		return GameMode->IsInteractble(Actor);
	}
	else 
	{
		return false;
	}
}

UInteractableComponent* AREM_Hud::GetInteractor(AActor* Actor)
{
	if (GameMode)
	{
		return GameMode->GetInteractor(Actor);
	}

	return nullptr;
}