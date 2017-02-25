// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#pragma once

#include "Components/ActorComponent.h"
#include "MenuIconsDef.h"
#include "StructsAndEnums.h"
#include "InteractableComponent.generated.h"

enum ButtonTypes
{
	EXAMINE,
	OPEN,
	PICKUP,
	USE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	// This function all interactable components inherit
	// This can be triggered from anywhere
	virtual void ActivateObject(AActor* Player);

	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	// The position relative to the object to move when activating the object.
	virtual FVector GetActivatePosition(AActor* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ShowAnimationBackwards = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RightclickMenu")
	bool ShowRightClickMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorMenuSnap")
	AActor* MenuSnapToActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanPlayerClick?")
	bool canPlayerClick = true;

	UFUNCTION(BlueprintCallable, Category = "MenuButtonArray")
	int GetNumMenuButtons();

	UFUNCTION(BlueprintCallable, Category = "MenuButtonArray")
	TArray<UTexture2D*> GetMenuButtonTextures(int i);

	TArray<UMenuIconsDef*> MenuButtons;
	TArray<UMenuIconsDef*> ObjectSpecificMenuButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuButtonArray")
	TArray<ActionType> Actions;

	UClass* SubMenuWidgetClassTemplate = nullptr;
	UUserWidget* SubMenuWidget = nullptr;
private:
};
