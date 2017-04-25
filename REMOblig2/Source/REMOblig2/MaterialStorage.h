// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

/**
 * 
 */
class REMOBLIG2_API MaterialStorage
{
public:
	MaterialStorage(TArray<FString> MaterialRef);
	~MaterialStorage();

	TArray<UMaterial*> Materials;
};
