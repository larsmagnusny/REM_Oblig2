// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool ShowAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

private:
	UClass* RightClickMenuClassTemplate = nullptr;
	UUserWidget* RightClickMenu = nullptr;
};
