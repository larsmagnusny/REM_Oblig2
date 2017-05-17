// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "Engine/GameInstance.h"
#include "Inventory.h"
#include "REMSaveGame.h"
#include "REM_GameInstance.generated.h"

/**
 * 
 */

enum
{
	ROOMLEVEL,
	HALLWAYLEVEL,
	NUM_LEVELS
};

UCLASS()
class REMOBLIG2_API UREM_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UREM_GameInstance();
	
	void BeginPlay();

	void DeletePersistentInventory();

	void DeleteLevelData(uint8 LevelIndex);

	UFUNCTION(BlueprintCallable, Category = "Clear all our persistent data...")
	void DeleteAllLevelData();

	void SaveAllData(REMSaveGame* SaveGameInstance);

	void LoadAllData(FName& LastLevel, REMSaveGame* SaveGameInstance);

	UFUNCTION(BlueprintCallable, Category = "Save Settings")
	void SaveSettings(AGameModeBase* GameMode);
	void LoadSettings(REMSaveGame* SaveGameInstance);

	UFUNCTION(BlueprintCallable, Category = "Check if savefile exists")
	bool FileExists(FString Path);

	UPROPERTY(BlueprintReadWrite, Category = "Tell the game if you are in main menu or not?!")
	bool MainMenu;
	bool LoadRoom;
	bool LoadCheckpoint;

	UPROPERTY(BlueprintReadWrite, Category = "If we should load the savefile next time we play")
	bool LoadCheckpointNextLevel = false;

	uint8 CurrentLevelLoaded = 0;

	// Persistent player inventory...
	FBufferArchive* PersistentInventory;

	FBufferArchive** LevelData;

	FBufferArchive* CombinedLevelData;

	FString CurrentLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Volume Control")
	float MasterVolume = 0.5f;
	UPROPERTY(BlueprintReadWrite, Category = "Volume Control")
	float SFXVolume = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Resolution")
	FVector2D Resolution = FVector2D(720, 1280);

	UPROPERTY(BlueprintReadWrite, Category = "Window Mode")
	uint8 WindowMode = EWindowMode::WindowedFullscreen;
};
