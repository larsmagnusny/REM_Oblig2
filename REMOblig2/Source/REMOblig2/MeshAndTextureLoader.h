// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once
#include "StructsAndEnums.h"
#include "MaterialStorage.h"

/**
 * 
 */

class REMOBLIG2_API MeshAndTextureLoader
{
public:
	MeshAndTextureLoader();
	~MeshAndTextureLoader();

	void Init();

	UPROPERTY(BlueprintCallable, Category = "Get Static Mesh")
	UStaticMesh* GetStaticMeshByItemID(ItemIDs ID);
	UTexture2D* GetTextureByItemID(ItemIDs ID);
	UPROPERTY(BlueprintCallable, Category = "Get Materials")
	TArray<UMaterial*> GetMaterialsByItemID(ItemIDs ID);
private:
	TArray<FString> MeshRefString;
	TArray<FString> ItemIconRefString;

	TArray<UStaticMesh*> StaticMeshes;
	TArray<UTexture2D*> ItemIconTextures;
	TArray<MaterialStorage*> MaterialReference;
};
