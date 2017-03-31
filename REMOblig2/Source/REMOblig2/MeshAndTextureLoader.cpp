// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MeshAndTextureLoader.h"

MeshAndTextureLoader::MeshAndTextureLoader()
{
	// Load Meshes and textures for inventoryItem
	MeshRefString.Add("StaticMesh'/Game/Meshes/Key.Key'");
	NumMats.Add(1);
	FString* Mats = new FString[NumMats[0]];
	Mats[0] = "Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'";
	MatRefStrings.Add(Mats);
	ItemIconRefString.Add("Texture2D'/Game/Textures/golkey.golkey'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/toy.toy'");
	NumMats.Add(1);
	Mats = new FString[NumMats[1]];
	Mats[0] = "Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'";
	MatRefStrings.Add(Mats);
	ItemIconRefString.Add("Texture2D'/Game/Textures/ToyIcon.ToyIcon'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Spinner_toy/Spinner_toy.Spinner_toy'");
	NumMats.Add(1);
	Mats = new FString[NumMats[1]];
	Mats[0] = "Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'";
	MatRefStrings.Add(Mats);
	ItemIconRefString.Add("Texture2D'/Game/Textures/SpinnerToy.SpinnerToy'");


	for (uint8 i = 0; i < MeshRefString.Num(); i++)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(*MeshRefString[i]);

		

		ConstructorHelpers::FObjectFinder<UTexture2D> TextureLoader(*ItemIconRefString[i]);

		if (MeshLoader.Succeeded() && TextureLoader.Succeeded())
		{
			StaticMeshes.Add(MeshLoader.Object);
			ItemIconTextures.Add(TextureLoader.Object);
		}
	}
}

MeshAndTextureLoader::~MeshAndTextureLoader()
{
}

UStaticMesh* MeshAndTextureLoader::GetStaticMeshByItemID(ItemIDs ID)
{
	return StaticMeshes[(uint8)ID];
}

UTexture2D* MeshAndTextureLoader::GetTextureByItemID(ItemIDs ID)
{
	return ItemIconTextures[(uint8)ID];
}