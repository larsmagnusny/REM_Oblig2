// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "GameFramework/SaveGame.h"
#include "REMSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API UREMSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UREMSaveGame();

	UPROPERTY(EditAnywhere, Category = Basic)
	uint32 DoorIndex;

	template<typename T> void SaveLoadData(FArchive& Ar, T& t)
	{
		Ar << t;
	}

	bool SaveGameDataToFile(const FString& FullFilePath, FBufferArchive& ToBinary);


private:
};
