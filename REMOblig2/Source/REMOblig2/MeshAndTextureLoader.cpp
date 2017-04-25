// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MeshAndTextureLoader.h"

MeshAndTextureLoader::MeshAndTextureLoader()
{
	// Load Meshes and textures for inventoryItem
	MeshRefString.Add("StaticMesh'/Game/Meshes/Key.Key'");
	TArray<FString> Mats;
	Mats.Add("Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'");

	MaterialReference.Add(new MaterialStorage(Mats));

	ItemIconRefString.Add("Texture2D'/Game/Textures/golkey.golkey'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/toy.toy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/ToyIcon.ToyIcon'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Spinner_toy/Spinner_toy.Spinner_toy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Meshes/Spinner_toy/SpinnerToyMat.SpinnerToyMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/SpinnerToy.SpinnerToy'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Pliers/pliers.pliers'");
	Mats.Empty();
	Mats.Add("Material'/Game/Meshes/Pliers/lambert1.lambert1'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/pliers.pliers'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Feather/Feather.Feather'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/FeatherMat.FeatherMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/FeatherIcon.FeatherIcon'");


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

TArray<UMaterial*> MeshAndTextureLoader::GetMaterialsByItemID(ItemIDs ID)
{
	return MaterialReference[(uint8)ID]->Materials;
}
