// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MeshAndTextureLoader.h"

MeshAndTextureLoader::MeshAndTextureLoader()
{
	// Load Meshes and textures for inventoryItem
	MeshRefString.Add("StaticMesh'/Game/Meshes/Key.Key'");
	ItemIconRefString.Add("Texture2D'/Game/Textures/golkey.golkey'");


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

UStaticMesh* GetStaticMeshByItemID(ItemIDs ID)
{
	return nullptr;
}

UTexture2D* GetTextureByItemID(ItemIDs ID)
{
	return nullptr;
}