// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "GameFramework/HUD.h"
#include "REM_Hud.generated.h"

/**
 * 
 */

UCLASS()
class REMOBLIG2_API AREM_Hud : public AHUD
{
	GENERATED_BODY()
	
public:
	AREM_Hud();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category="ActivateObect")
	void CallActivate(ActionType Action);

	UFUNCTION(BlueprintCallable, Category = "ItemInteract")
	void ItemInteract(UInteractableComponent* Component, int SlotNum);

	// Add widget to actorcomponent
	void AddInteractionWidget(AActor* OwnerObject, UUserWidget* Widget, UInteractableComponent* Component, AInteractableObject* Object = nullptr);

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "GetInteractionWidget")
	UInteractableComponent* GetParentInteractor(UUserWidget* Widget);

	// For � hente Den tilknyttede interactoren til widgeten
	InteractionWidget* GetParentInteractorI(UUserWidget* Widget);
	InteractionWidget* GetParentInteractorI(AActor* Owner);

	// For � lage en widget fra en klasse som ikke kan lage en widget
	UUserWidget* HUDCreateWidget(UClass* Template);

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool ShowAnimation = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

	AActor* HintSnapToActor = nullptr;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool canPlayerClick = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue menu open")
	bool DialogueMenuOpen = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	// Den menyen som er �pen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Menu Widget")
	UUserWidget* RightClickMenu = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Set Hud Widget to null")
	void ResetRightClickMenu();

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "Get Slot Number")
	void GetSlotNumber(int32& num) { num = SlotCounter++; }

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "Swap Inventory Elements")
	void SwapElements(int32 index1, int32 index2);

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "Drop Item")
	void DropItem(int32 SlotIndex, FVector2D HitPoint);

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "Check If we can interact with the object...")
	bool IsActorInteractable(AActor* Actor);

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "Get the interactor instance")
	UInteractableComponent* GetInteractor(AActor* Actor);

	void RemoveInteractionWidget(UInteractableComponent* Component);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool MainMenuLevel = true;

	void TogglePauseMenuVisibility();

	UPROPERTY(BlueprintReadWrite, Category = "MainMenuWidget")
	UUserWidget* MainMenuWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TotalFadeValue")
	float CurrentOpacity = 0.f;

	// Only way to tell the bookpuzzle who owns him at the moment...
	// Should set up a baseclass for the blueprint...
	UPROPERTY(BlueprintReadWrite)
	UActorComponent* BookPuzzleParent = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Pointer to ToggleObjectDisplay")
	bool GetToggleObjectDisplay()
	{
		return ToggleObjectDisplay;
	}
	UFUNCTION(BlueprintCallable, Category = "Pointer to ToggleObjectDisplay")
	void SetToggleObjectDisplay(bool val)
	{
		ToggleObjectDisplay = val;
	}

	bool ToggleObjectDisplay = false;

	UPROPERTY(BlueprintReadOnly)
	ItemIDs ItemToDisplay = ItemIDs::ITEM_SPINNER;

	UPROPERTY(BlueprintReadWrite, Category = "ShouldDisplayTip")
	bool DisplayTip = false;

	UFUNCTION(BlueprintCallable, Category = "Set StaticMesh Movable...")
	void SetMovable(AStaticMeshActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Custom Conversion")
	FString IntegerToString(int Integer)
	{
		FString String = "0";

		if (Integer == 0)
		{
			String[0] = (char)32;
		}
		else {
			String[0] = (char)('A' + Integer - 1);
		}

		return String;
	}

	UPROPERTY(BlueprintReadWrite, Category = "Credits")
	AActor* CreditsActor = nullptr;

private:
	bool SubMenuesInUse = false;


	UClass* InventoryWidgetClassTemplate = nullptr;
	UUserWidget* InventoryWidget = nullptr;

	UClass* MainMenuWidgetClassTemplate = nullptr;
	

	UClass* PauseMenuWidgetClassTemplate = nullptr;
	UUserWidget* PauseMenuWidget = nullptr;

	UClass* UserTipsWidgetClassTemplate = nullptr;
	UUserWidget* UserTipsWidget = nullptr;

	UClass* CreditsWidgetClassTemplate = nullptr;
	UUserWidget* CreditsWidget = nullptr;

	TArray<InteractionWidget> SubMenues;

	TArray<UImage*> Slots;

	AREM_GameMode* GameMode = nullptr;
	UREM_GameInstance* GameInstance = nullptr;

	// Keep track of our inventory slots
	int32 SlotCounter = 0;
};
