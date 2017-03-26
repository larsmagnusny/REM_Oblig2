// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Engine/GameInstance.h"
#include "REM_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API UREM_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UREM_GameInstance();
	
	void BeginPlay();

	UPROPERTY(BlueprintReadWrite, Category = "Tell the game if you are in main menu or not?!")
	bool MainMenu;
	bool LoadRoom;
	bool LoadCheckpoint;

	FString CurrentLevel;
};
