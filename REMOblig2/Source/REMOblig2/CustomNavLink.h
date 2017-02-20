// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#pragma once

#include "AI/Navigation/NavLinkProxy.h"
#include "CustomNavLink.generated.h"

/**
 * 
 */
UCLASS()
class REMOBLIG2_API ACustomNavLink : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	
};
