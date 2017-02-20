// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "REM_Hud.h"
<<<<<<< HEAD




AREM_Hud::AREM_Hud()
{

}

void AREM_Hud::BeginPlay()
{
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;
}
=======
>>>>>>> 3f4b62b2900fe2b404f68c2ba0b2660ccffac8cb
