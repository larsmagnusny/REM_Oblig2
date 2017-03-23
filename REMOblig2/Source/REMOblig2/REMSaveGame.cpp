// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REMSaveGame.h"

UREMSaveGame::UREMSaveGame()
{

}

bool UREMSaveGame::SaveGameDataToFile(const FString & FullFilePath, FBufferArchive & ToBinary)
{
	SaveLoadData(ToBinary, DoorIndex);

	if (ToBinary.Num() <= 0)
		return false;

	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		// Free Binary Array
		ToBinary.FlushCache();
		ToBinary.Empty();

		return true;
	}

	// Free binary array
	ToBinary.FlushCache();
	ToBinary.Empty();

	return false;
}
