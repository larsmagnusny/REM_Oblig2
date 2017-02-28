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

	UFUNCTION(BlueprintCallable, Category="ActivateObect")
	void CallActivate(ActionType Action);

	// Add widget to actorcomponent
	void AddInteractionWidget(AActor* OwnerObject, UUserWidget* Widget, UInteractableComponent* Component, AInteractableObject* Object = nullptr);

	UFUNCTION(BlueprintCallable, Category = "GetInteractionWidget")
	UInteractableComponent* GetParentInteractor(UUserWidget* Widget);

	InteractionWidget* GetParentInteractorI(UUserWidget* Widget);
	InteractionWidget* GetParentInteractorI(AActor* Owner);

	UUserWidget* HUDCreateWidget(UClass* Template);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool ShowAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool canPlayerClick = true;

	UUserWidget* RightClickMenu = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Get Slot Number")
	void GetSlotNumber(int32& num) { num = SlotCounter++; }

	UFUNCTION(BlueprintCallable, Category = "Swap Inventory Elements")
	void SwapElements(int32 index1, int32 index2);

	UFUNCTION(BlueprintCallable, Category = "Drop Item")
	void DropItem(int32 SlotIndex, FVector2D HitPoint);

	UFUNCTION(BlueprintCallable, Category = "Check If we can interact with the object...")
	bool IsActorInteractable(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Get the interactor instance")
	UInteractableComponent* GetInteractor(AActor* Actor);

private:
	UClass* InventoryWidgetClassTemplate = nullptr;
	UUserWidget* InventoryWidget = nullptr;
	TArray<InteractionWidget> SubMenues;

	TArray<UImage*> Slots;

	AREM_GameMode* GameMode = nullptr;

	// Keep track of our inventory slots
	int32 SlotCounter = 0;
};
