// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameInstance.h"

UREM_GameInstance::UREM_GameInstance()
{
	MainMenu = true;
	LoadRoom = false;
	LoadCheckpoint = false;

	UE_LOG(LogTemp, Warning, TEXT("Constructor in gameinstance ran!"));
}

void UREM_GameInstance::BeginPlay()
{
	// When the gamemode calls beginplay... we should check what level we are in...
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	CurrentLevel = LevelName;

	if (CurrentLevel.Equals("MainMenu"))
	{
		MainMenu = true;
	}
}
