// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "MenuIconsDef.h"

UMenuIconsDef::UMenuIconsDef(FString NormalT, FString HoverT, FString ClickT)
{
	ConstructorHelpers::FObjectFinder<UTexture2D> Normal(*NormalT);
	ConstructorHelpers::FObjectFinder<UTexture2D> Hover(*HoverT);
	ConstructorHelpers::FObjectFinder<UTexture2D> Click(*ClickT);

	Textures[0] = Normal.Object;
	Textures[1] = Hover.Object;
	Textures[2] = Click.Object;
}

UMenuIconsDef::~UMenuIconsDef()
{

}
