// © 2014 - 2016 Soverance Studios. All Rights Reserved.

#include "Ethereal.h"
#include "Magic_Master.h"


void AMagic_Master::QuitCharging()
{
	IsCharging = false;
	ChargeFX->DeactivateSystem();
}
