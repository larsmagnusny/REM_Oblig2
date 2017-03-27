// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_GameInstance.h"

UREM_GameInstance::UREM_GameInstance()
{
	MainMenu = true;
	LoadRoom = false;
	LoadCheckpoint = false;
	PersistentInventory = new FBufferArchive();

	LevelData = new FBufferArchive*[NUM_LEVELS];

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		LevelData[i] = new FBufferArchive();
	}

	UE_LOG(LogTemp, Warning, TEXT("Constructor in gameinstance ran!"));
}

void UREM_GameInstance::BeginPlay()
{
	// When the gamemode calls beginplay... we should check what level we are in...
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	CurrentLevel = LevelName;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentLevel);

	if (CurrentLevel.Equals("MainMenu"))
	{
		MainMenu = true;
	}
	else
	{
		MainMenu = false;
	}

	if (CurrentLevel.Equals("RoomLevel"))
	{
		CurrentLevelLoaded = ROOMLEVEL;
	}
	if (CurrentLevel.Equals("HallwayLevel"))
	{
		CurrentLevelLoaded = HALLWAYLEVEL;
	}
}

void UREM_GameInstance::DeletePersistentInventory()
{
	PersistentInventory->FlushCache();
	PersistentInventory->Empty();
	delete PersistentInventory;

	PersistentInventory = new FBufferArchive();
}

void UREM_GameInstance::DeleteLevelData(uint8 LevelIndex)
{
	LevelData[LevelIndex]->FlushCache();
	LevelData[LevelIndex]->Empty();
	//delete LevelData[LevelIndex];

	LevelData[LevelIndex] = new FBufferArchive();
}

void UREM_GameInstance::SaveAllData(REMSaveGame * SaveGameInstance)
{
	FBufferArchive TotalSaveData;

	FName LastLevel = *CurrentLevel;

	TotalSaveData << *PersistentInventory;

	TotalSaveData << LastLevel;

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		TotalSaveData << *LevelData[i];
	}

	SaveGameInstance->SaveGameDataToFile("SaveFile", TotalSaveData);
}

void UREM_GameInstance::LoadAllData(FName & LastLevel, REMSaveGame* SaveGameInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to load all data..."));
	FBufferArchive TotalSaveData;
	SaveGameInstance->LoadGameDataFromFile("SaveFile", TotalSaveData);

	UE_LOG(LogTemp, Warning, TEXT("Data Loaded from file..."));

	FMemoryReader Ar = FMemoryReader(TotalSaveData, false);

	DeletePersistentInventory();
	UE_LOG(LogTemp, Warning, TEXT("Cleared persistent memory"));
	Ar << *PersistentInventory;
	UE_LOG(LogTemp, Warning, TEXT("Read data from save file into inventory"));

	Ar << LastLevel;
	UE_LOG(LogTemp, Warning, TEXT("Loaded Last level"));
	
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		DeleteLevelData((uint8)i);
		UE_LOG(LogTemp, Warning, TEXT("Deleted LevelData Cache"));
		Ar << *LevelData[i];
		UE_LOG(LogTemp, Warning, TEXT("LevelData sucessfully loaded"));
	}
}

bool UREM_GameInstance::FileExists(FString Path)
{
	return FPaths::FileExists(Path);
}
