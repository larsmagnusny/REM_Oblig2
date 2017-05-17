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

	MeshRefString.Add("StaticMesh'/Game/Meshes/LydLeke/lydleke.fuuuuuuuck'");
	Mats.Empty();
	Mats.Add("Material'/Game/Meshes/LydLeke/Mat.Mat'");
	Mats.Add("Material'/Game/Meshes/LydLeke/Mat.Mat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/SoundToy.SoundToy'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1blue.kloss1blue'");
	Mats.Empty();

	for(int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Blue.Blue'");

	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/blue_lego.blue_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1gul.kloss1gul'");
	Mats.Empty();
	for (int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Gul.Gul'");

	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/yellow_lego.yellow_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1rosa.kloss1rosa'");
	Mats.Empty();

	for (int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Rosa.Rosa'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/pink_lego.pink_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2green.kloss2green'");
	Mats.Empty();

	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Green.Green'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/green_lego.green_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2lilla.kloss2lilla'");
	Mats.Empty();

	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Lilla.Lilla'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/purple_lego.purple_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2orange.kloss2orange'");
	Mats.Empty();
	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Meshes/Klosser/Orange.Orange'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/orange_lego.orange_lego'");


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
