// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "REM_GameMode.h"
#include "REM_Hud.h"
#include "InteractableComponent.h"
#include "ChestController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMOBLIG2_API UChestController : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChestController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Disse funksjonene er definert i UInteractableComponent klassen
	// Vi bruker polymorfi fordi det er lettere å inkludere en generell fil for å så kalle disse funksjonene
	// For å "snakke" med klassen ifra andre klasser
	virtual void ActivateObject(AActor* Player) override;
	virtual void ExamineObject(AActor* Player) override;
	virtual void OpenInventory(AActor* Player) override;
	virtual void PickupObject(AActor* Player) override;
	virtual void ActivateDialogue(AActor* Player) override;
	virtual void DialogueOptionPressed(UUserWidget* Caller, int optionindex) override;
	virtual void ItemInteract(int32 SlotNum) override;

	// En Funksjon som kalles ifra blueprint for å vite hvor kisten skal gå når du sier at den skal gå til et sted
	UFUNCTION(BlueprintCallable, Category = "Dialogue Pressed")
	AActor* SlotToGoTo();

	// Kalles ifra en blueprint, forteller kisten at den skal "snappe" til Actoren vi har blitt fortalt om å gå til
	UFUNCTION(BlueprintCallable, Category = "Dialogue Pressed")
	void SnapToSlot();

	// Variabel som brukes i AnimasjonsBlueprint for å se om kisten skal være åpen eller ikke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Is it open?")
	bool isOpen = false;

	// Når vi "Aktiverer" kisten så følger den etter oss
	UPROPERTY(EditAnywhere)
	bool FollowWhenActivate = true;

	// Kisten skal aktivt følge etter spilleren hvis denne er true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FollowPlayer?")
	bool FollowingPlayer = false;

	// Er kisten låst?
	UPROPERTY(EditAnywhere)
	bool locked = true;

	// Den INTERACT_IDen eventuelt nøkkelen må ha for å åpne kisten
	UPROPERTY(EditAnywhere)
	int OPENID = 1337;

	// Peker til spilleren den skal følge etter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player To follow")
	AActor* PlayerToFollow = nullptr;

	// Peker til HUD klassen
	AREM_Hud* Hud = nullptr;

	// Peker til GameMode Klassen
	AREM_GameMode* GameMode = nullptr;

	// A place where the chest can sit...
	UPROPERTY(EditAnywhere)
	AActor* Slot1 = nullptr;

	// A place where the chest can sit
	UPROPERTY(EditAnywhere)
	AActor* Slot2 = nullptr;

	// Boolean for at blueprintet til AI'en skal vite hvem han skal følge etter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Should i try to go to the slot?")
	bool GoToSlot = false;
	
	// Generelle variabler vi bruker
	int Slot = -1;
	bool SnappedToSlot = false;
	FRotator Rotation;
	FVector SlotLocation;

	// Brukes for å "se" om kisten er full ifra andre klasser
	bool filled = false;
private:
	// Tell hvor mange leker vi har puttet i kisten
	int toysfilled = 0;
};
