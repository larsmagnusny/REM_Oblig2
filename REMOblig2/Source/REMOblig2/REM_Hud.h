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

	// Add widget to actorcomponent
	void AddInteractionWidget(AActor* OwnerObject, UUserWidget* Widget, UInteractableComponent* Component, AInteractableObject* Object = nullptr);

	// Brukes i Blueprint
	UFUNCTION(BlueprintCallable, Category = "GetInteractionWidget")
	UInteractableComponent* GetParentInteractor(UUserWidget* Widget);

	// For å hente Den tilknyttede interactoren til widgeten
	InteractionWidget* GetParentInteractorI(UUserWidget* Widget);
	InteractionWidget* GetParentInteractorI(AActor* Owner);

	// For å lage en widget fra en klasse som ikke kan lage en widget
	UUserWidget* HUDCreateWidget(UClass* Template);

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool ShowAnimation = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

	// Brukes i Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool canPlayerClick = true;

	// Den menyen som er åpen
	UUserWidget* RightClickMenu = nullptr;

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
private:
	UClass* InventoryWidgetClassTemplate = nullptr;
	UUserWidget* InventoryWidget = nullptr;

	UClass* MainMenuWidgetClassTemplate = nullptr;
	UUserWidget* MainMenuWidget = nullptr;

	TArray<InteractionWidget> SubMenues;

	TArray<UImage*> Slots;

	AREM_GameMode* GameMode = nullptr;

	// Keep track of our inventory slots
	int32 SlotCounter = 0;
};
