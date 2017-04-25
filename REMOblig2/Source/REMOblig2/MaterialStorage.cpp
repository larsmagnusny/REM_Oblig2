// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MaterialStorage.h"

MaterialStorage::MaterialStorage(TArray<FString> MaterialRef)
{
	for (FString ref : MaterialRef)
	{
		ConstructorHelpers::FObjectFinder<UMaterial> MatLoader(*ref);

		if (MatLoader.Succeeded())
		{
			Materials.Add(MatLoader.Object);
		}
	}
}

MaterialStorage::~MaterialStorage()
{
}
