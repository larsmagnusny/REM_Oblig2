// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "REM_Hud.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ActionType : uint8
{
	INTERACT_EXAMINE UMETA(DisplayName = "Examine the object"),
	INTERACT_OPENINVENTORY UMETA(DisplayName = "Open Inventory"),
	INTERACT_PICKUP UMETA(DisplayName = "Pickup Object"),
	INTERACT_ACTIVATE UMETA(DisplayName = "Activate or Use Object")
};

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

private:
	UClass* RightClickMenuClassTemplate = nullptr;
	UUserWidget* RightClickMenu = nullptr;
};
