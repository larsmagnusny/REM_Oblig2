// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "InteractableComponent.h"
#include "InteractableObject.h"
#include "StructsAndEnums.h"
#include "Blueprint/UserWidget.h"
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

private:
	UClass* RightClickMenuClassTemplate = nullptr;

	TArray<InteractionWidget> SubMenues;
};
