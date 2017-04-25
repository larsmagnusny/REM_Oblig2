// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"
#include "MainCharacter.h"
#include "REM_Hud.h"

AREM_Hud::AREM_Hud()
{
	// Last inn Inventory Widgeten
	ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetLoader(TEXT("WidgetBlueprint'/Game/UI/Inventory.Inventory_C'"));
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetLoader(TEXT("WidgetBlueprint'/Game/UI/MainMenu.MainMenu_C'"));
	ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuWidgetLoader(TEXT("WidgetBlueprint'/Game/UI/PauseMenu.PauseMenu_C'"));
	ConstructorHelpers::FClassFinder<UUserWidget> UserTipsWidgetLoader(TEXT("WidgetBlueprint'/Game/UI/UserTipsWidget.UserTipsWidget_C'"));

	if (InventoryWidgetLoader.Succeeded())
	{
		InventoryWidgetClassTemplate = InventoryWidgetLoader.Class;
	}

	if (MainMenuWidgetLoader.Succeeded())
	{
		MainMenuWidgetClassTemplate = MainMenuWidgetLoader.Class;
	}

	if (PauseMenuWidgetLoader.Succeeded())
	{
		PauseMenuWidgetClassTemplate = PauseMenuWidgetLoader.Class;
	}

	if (UserTipsWidgetLoader.Succeeded())
	{
		UserTipsWidgetClassTemplate = UserTipsWidgetLoader.Class;
	}
}

void AREM_Hud::BeginPlay()
{
	GameInstance = Cast<UREM_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	// Spilleren skal kunne trykke på skjermen
	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;

	// Lag InventoryWidgeten og legg den til viewporten
	if (InventoryWidgetClassTemplate)
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InventoryWidgetClassTemplate);

		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport(0);
			// Hent pekerene til InventorySlottene i InventoryBlueprintet
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
		}
	}

	if (MainMenuWidgetClassTemplate)
	{
		MainMenuWidget = CreateWidget < UUserWidget>(GetWorld()->GetFirstPlayerController(), MainMenuWidgetClassTemplate);

		MainMenuWidget->AddToViewport(1);
	}

	if (PauseMenuWidgetClassTemplate)
	{
		PauseMenuWidget = CreateWidget < UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseMenuWidgetClassTemplate);

		PauseMenuWidget->AddToViewport(2);

		PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PauseMenuWidgetClassTemplate)
	{
		UserTipsWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), UserTipsWidgetClassTemplate);

		UserTipsWidget->AddToViewport(3);
		//UserTipsWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// Hent en peker til GameMode
	GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());
}

void AREM_Hud::DrawHUD()
{
	Super::DrawHUD();

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt((int32)GameInstance->MainMenu));

	if (HintSnapToActor && !MainMenuLevel)
	{
		UserTipsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), HintSnapToActor->GetActorLocation(), ScreenPos, false);

		ScreenPos += FVector2D(-25, -35)*ViewportScale;

		UserTipsWidget->SetPositionInViewport(ScreenPos, true);
		UE_LOG(LogTemp, Error, TEXT("Showing!"));
	}
	else if (!HintSnapToActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Not Showing!"));
		UserTipsWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(!GameInstance->MainMenu) {
		MainMenuLevel = false;

		if (MainMenuWidget->GetIsVisible())
			MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// Hent en peker til MainCharacter
	AMainCharacter* MainCharacter = nullptr;
	if(GameMode && !MainMenuLevel)
		MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	if (CurrentOpacity > 0.8f)
	{
		if (InventoryWidget->IsVisible())
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (CurrentOpacity < 0.8f)
	{
		if (!InventoryWidget->IsVisible())
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (MainCharacter && !MainMenuLevel)
	{
		// Sett Brushen til bildene i InventorySlot til en 2D tekstur basert på om det er en item der eller ikke
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

	if (!RightClickMenu && !MainMenuLevel)
	{
		return;
	}

	// For å sette posisjonen til en meny basert på hvor den er i forhold til kameraet
	if (MenuSnapToActor && !MainMenuLevel)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), MenuSnapToActor->GetActorLocation(), ScreenPos, false);
		
		ScreenPos += FVector2D(-160, -160)*ViewportScale;

		RightClickMenu->SetPositionInViewport(ScreenPos, true);
	}

	
}

void AREM_Hud::CallActivate(ActionType Action)
{
	// For å håntere hvilken knapp du trykte på og sende kommandoen videre til InteractableObject
	InteractableObject* Obj = GameMode->GetInteractableObject(MenuSnapToActor);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GameMode->GetMainCharacter());

	if(Obj->ScriptComponent)
		MainCharacter->DelayCallFunctionFromWidget(Obj->ScriptComponent->GetActivatePosition(MainCharacter), Obj, Action);
	if (Obj->StaticMeshInstance)
		MainCharacter->DelayCallFunctionFromWidget(Obj->StaticMeshInstance->GetActivatePosition(), Obj, Action);

	return;
}

void AREM_Hud::AddInteractionWidget(AActor* OwnerObject, UUserWidget* Widget, UInteractableComponent* Component, AInteractableObject* Object)
{
	SubMenuesInUse = true;
	InteractionWidget InterWidget;
	InterWidget.Owner = OwnerObject;
	InterWidget.MenuWidget = Widget;
	InterWidget.ParentComponent = Component;
	InterWidget.ParentObject = Object;
	SubMenues.Add(InterWidget);
	SubMenuesInUse = false;
}

void AREM_Hud::RemoveInteractionWidget(UInteractableComponent* Component)
{
	for(int i = 0; i < SubMenues.Num(); i++)
	{
		if (SubMenues[i].ParentComponent == Component)
		{
			SubMenues[i].MenuWidget->SetVisibility(ESlateVisibility::Hidden);

			SubMenues.RemoveAt(i);

			canPlayerClick = true;

			SubMenuesInUse = false;
			return;
		}
	}
}

void AREM_Hud::TogglePauseMenuVisibility()
{
	if (PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	if (PauseMenuWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AREM_Hud::SetMovable(AStaticMeshActor * Actor)
{
	UStaticMeshComponent* Component = Actor->StaticMeshComponent;
	Component->SetMobility(EComponentMobility::Movable);
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

void AREM_Hud::ResetRightClickMenu()
{
	RightClickMenu = nullptr;
	ShowRightClickMenu = false;
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
	{
		// Drop item if we get a hitresult under the item...
		FHitResult Hit[6];
		FVector Start = MainCharacter->GetActorLocation();

		Start.X = HitPoint.X;
		Start.Y = HitPoint.Y;

		GetWorld()->LineTraceSingleByChannel(Hit[0], Start, Start + FVector(1, 0, 0)*10000.f, ECollisionChannel::ECC_Visibility);
		GetWorld()->LineTraceSingleByChannel(Hit[1], Start, Start + FVector(0, 1, 0)*10000.f, ECollisionChannel::ECC_Visibility);
		GetWorld()->LineTraceSingleByChannel(Hit[2], Start, Start + FVector(-1, 0, 0)*10000.f, ECollisionChannel::ECC_Visibility);
		GetWorld()->LineTraceSingleByChannel(Hit[3], Start, Start + FVector(0, -1, 0)*10000.f, ECollisionChannel::ECC_Visibility);
		GetWorld()->LineTraceSingleByChannel(Hit[4], Start, Start + FVector(0, 0, 1)*10000.f, ECollisionChannel::ECC_Visibility);
		GetWorld()->LineTraceSingleByChannel(Hit[5], Start, Start + FVector(0, 0, -1)*10000.f, ECollisionChannel::ECC_Visibility);

		float Distances[6];

		for (int i = 0; i < 6; i++)
		{
			if (Hit[i].GetActor())
			{
				Distances[i] = (Hit[i].ImpactPoint - Start).Size();
			}
			else {
				Distances[i] = 10000.f;
			}

			UE_LOG(LogTemp, Warning, TEXT("RayCastLength: %s"), *FString::SanitizeFloat(Distances[i]));
		}

		for (int i = 0; i < 4; i++)
		{
			if (Distances[i] == 0.f)
			{
				UE_LOG(LogTemp, Error, TEXT("Inside OBJECT!"));
				return;
			}
		}

		//if (Distances[0] < 50.f && Distances[2] < 50.f)
			//return;
		//if (Distances[1] < 50.f && Distances[3] < 50.f)
			//return;
		if (Distances[4] < 10000.f)
		{
			UE_LOG(LogTemp, Error, TEXT("Inside OBJECT!"));
			return;
		}
		MainCharacter->DropItem(SlotIndex, HitPoint);
	}
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