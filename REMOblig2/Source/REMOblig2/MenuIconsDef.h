// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

/**
 * 
 */

class REMOBLIG2_API UMenuIconsDef
{
	
public:
	UMenuIconsDef(FString NormalT, FString HoverT, FString ClickT);
	~UMenuIconsDef();

	// Three possible textures
	int size = 3;
	UTexture2D* Textures[3];
};
