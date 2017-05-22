// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MeshAndTextureLoader.h"

MeshAndTextureLoader::MeshAndTextureLoader()
{
	// Load Meshes and textures for inventoryItem
	MeshRefString.Add("StaticMesh'/Game/Meshes/PlaceHolder/Key.Key'");
	TArray<FString> Mats;
	Mats.Add("Material'/Game/Materials/SpinnerToyMat.SpinnerToyMat'");

	MaterialReference.Add(new MaterialStorage(Mats));

	ItemIconRefString.Add("Texture2D'/Game/Textures/golkey.golkey'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/PlaceHolder/toy.toy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/SpinnerToyMat.SpinnerToyMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/ToyIcon.ToyIcon'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Spinner_toy/Spinner_toy.Spinner_toy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/SpinnerToyMat.SpinnerToyMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/SpinnerToy.SpinnerToy'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Pliers/pliers.pliers'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/PlierMat.PlierMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/pliers.pliers'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Feather/Feather.Feather'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/FeatherMat.FeatherMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/FeatherIcon.FeatherIcon'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/LydLeke/SoundToy.SoundToy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/SoundToyMat.SoundToyMat'");
	Mats.Add("Material'/Game/Materials/SoundToyMat.SoundToyMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/SoundToy.SoundToy'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1blue.kloss1blue'");
	Mats.Empty();

	for(int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Materials/Blue.Blue'");

	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/blue_lego.blue_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1gul.kloss1gul'");
	Mats.Empty();
	for (int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Materials/Gul.Gul'");

	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/yellow_lego.yellow_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss1rosa.kloss1rosa'");
	Mats.Empty();

	for (int i = 0; i < 5; i++)
		Mats.Add("Material'/Game/Materials/Rosa.Rosa'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/pink_lego.pink_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2green.kloss2green'");
	Mats.Empty();

	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Materials/Green.Green'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/green_lego.green_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2lilla.kloss2lilla'");
	Mats.Empty();

	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Materials/Lilla.Lilla'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/purple_lego.purple_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Klosser/kloss2orange.kloss2orange'");
	Mats.Empty();
	for (int i = 0; i < 7; i++)
		Mats.Add("Material'/Game/Materials/Orange.Orange'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/orange_lego.orange_lego'");

	MeshRefString.Add("StaticMesh'/Game/Meshes/Radio/RadioStatic.RadioStatic'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/RadioMat2.RadioMat2'");
	Mats.Add("Material'/Game/Materials/RadioMat1.RadioMat1'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/RadioIcon.RadioIcon'");

	// Rubix cube
	MeshRefString.Add("StaticMesh'/Game/Meshes/Rubix/rubiks_cube.rubiks_cube'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/RubixCube.RubixCube'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/rubix.rubix'");

	// Teddy
	MeshRefString.Add("StaticMesh'/Game/Meshes/Teddy/Teddy.Teddy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/TeddyMat2.TeddyMat2'");
	Mats.Add("Material'/Game/Materials/TeddyMat1.TeddyMat1'");
	Mats.Add("Material'/Game/Materials/TeddyMat3.TeddyMat3'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/teddy.teddy'");

	// Stacking toy
	MeshRefString.Add("StaticMesh'/Game/Meshes/StackingToy/StackingToy.StackingToy'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/BaseColor.BaseColor'");
	Mats.Add("Material'/Game/Materials/Color2.Color2'");
	Mats.Add("Material'/Game/Materials/Color3.Color3'");
	Mats.Add("Material'/Game/Materials/Color4.Color4'");
	Mats.Add("Material'/Game/Materials/Color5.Color5'");
	Mats.Add("Material'/Game/Materials/Color1.Color1'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/stacking.stacking'");

	// Wooden doll
	MeshRefString.Add("StaticMesh'/Game/Meshes/Thing/StaticMesh.StaticMesh'");
	Mats.Empty();
	Mats.Add("Material'/Game/Materials/ThingMat.ThingMat'");
	MaterialReference.Add(new MaterialStorage(Mats));
	ItemIconRefString.Add("Texture2D'/Game/Textures/thing.thing'");

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
