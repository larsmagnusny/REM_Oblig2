// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "StructsAndEnums.h"

/**
 * 
 */
class REMOBLIG2_API MeshAndTextureLoader
{
public:
	MeshAndTextureLoader();
	~MeshAndTextureLoader();

	void Init();

	UStaticMesh* GetStaticMeshByItemID(ItemIDs ID);
	UTexture2D* GetTextureByItemID(ItemIDs ID);
private:
	TArray<UStaticMesh*> StaticMeshes;
	TArray<FString> MeshRefString;

	TArray<UTexture2D*> ItemIconTextures;
	TArray<FString> ItemIconRefString;
};
