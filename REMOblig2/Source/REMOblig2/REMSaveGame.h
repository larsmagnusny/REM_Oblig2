// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

class REMOBLIG2_API REMSaveGame
{
public:
	REMSaveGame();

	bool SaveGameDataToFile(const FString& FullFilePath, FBufferArchive& ToBinary);
	bool LoadGameDataFromFile(const FString& FullFilePath, FBufferArchive& BinaryData);


private:
};
