// © 2014 - 2016 Soverance Studios. All Rights Reserved.

#pragma once
#include "WeaponModes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponModes : uint8
{
	WM_None UMETA(DisplayName = "NONE"),
	WM_OneHanded UMETA(DisplayName = "One-Handed"),
	WM_TwoHanded UMETA(DisplayName = "Two-Handed"),
	WM_Ranged UMETA(DisplayName = "Ranged"),
	WM_Casting UMETA(DisplayName = "Casting"),
};
