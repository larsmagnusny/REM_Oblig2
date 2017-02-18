// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "GameFramework/HUD.h"
#include "REM_Hud.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API AREM_Hud : public AHUD
{
	GENERATED_BODY()
	
public:
	AREM_Hud();
	virtual void BeginPlay() override;
	
};
