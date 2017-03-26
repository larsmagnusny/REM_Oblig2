// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REMSaveGame.h"

REMSaveGame::REMSaveGame()
{

}

bool REMSaveGame::SaveGameDataToFile(const FString & FullFilePath, FBufferArchive & ToBinary)
{
	if (ToBinary.Num() <= 0)
		return false;

	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		// Free Binary Array
		ToBinary.FlushCache();
		ToBinary.Empty();

		UE_LOG(LogTemp, Warning, TEXT("File %s successfully saved..."), *FullFilePath);

		return true;
	}

	// Free binary array
	ToBinary.FlushCache();
	ToBinary.Empty();

	return false;
}

bool REMSaveGame::LoadGameDataFromFile(const FString & FullFilePath, FBufferArchive & BinaryData)
{
	if (FFileHelper::LoadFileToArray(BinaryData, *FullFilePath))
	{

		return true;
	}

	return false;
}
