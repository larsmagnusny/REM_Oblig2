// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)


class REMOBLIG2_API REMSaveGame
{
public:
	REMSaveGame();

	template<typename T> void SaveLoadData(FArchive& Ar, T& t)
	{
		Ar << t;
	}

	bool SaveGameDataToFile(const FString& FullFilePath, FBufferArchive& ToBinary);
	bool LoadGameDataFromFile(const FString& FullFilePath, FBufferArchive& BinaryData);


private:
};
