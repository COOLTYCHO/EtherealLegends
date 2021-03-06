// © 2014 - 2016 Soverance Studios
// http://www.soverance.com

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Ethereal.h"
#include "Characters/Player/EtherealPlayerMaster.h"
#include "Blizzard.h"

// Sets default values
ABlizzard::ABlizzard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Get Assets, References Obtained Via Right Click in Editor
	ConstructorHelpers::FObjectFinder<UParticleSystem> ChargeParticleObject(TEXT("ParticleSystem'/Game/Blueprints/Gear/Magic/SpellEffects/Particles/ChargeFX/ChargeParticle.ChargeParticle'"));
	ConstructorHelpers::FObjectFinder<UTexture2D> LargeIconObject(TEXT("Texture2D'/Game/Blueprints/Widgets/UI-Images/Icons_Magic/blizzard.blizzard'"));
	ConstructorHelpers::FObjectFinder<UTexture2D> SmallIconObject(TEXT("Texture2D'/Game/Blueprints/Widgets/UI-Images/Icons_Magic/blizzard-small.blizzard-small'"));

	// Set Default Values
	Name = EMasterGearList::GL_Blizzard;
	Type = EMasterGearTypes::GT_Black;
	Description = "Briefly encase a single target in a block of ice, halting their movement.";
	MPCost = 60;
	ATK = 5;
	DEF = 0;
	SPD = 0;
	HP = -120;
	MP = 40;
	Duration = 10;
	CastTime = 30;
	CritMultiplier = 0;
	HasteMultiplier = 0;
	DefenseMultiplier = 0;
	TargetType = EMagic_TargetTypes::TT_Enemy;
	AnimType = EMagic_AnimTypes::AT_GroundUp;

	// Set Default Objects
	P_ChargeFX = ChargeParticleObject.Object;
	LargeIcon = LargeIconObject.Object;
	SmallIcon = SmallIconObject.Object;

	ChargeFX->Template = P_ChargeFX;
}

// Called when the game starts or when spawned
void ABlizzard::BeginPlay()
{
	Super::BeginPlay();

	// Bind the Use function to the event dispatcher for Item Use
	QuitCharging.AddDynamic(this, &ABlizzard::Cancel);
		
}

void ABlizzard::Cancel()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Blizzard casting was cancelled.");
}

void ABlizzard::ChargeBlizzard()
{
	IsCharging = true; // Set Charging State True
	ChargeFX->SetColorParameter("Color", FLinearColor(0.23f, 0.71f, 1, 1));  // Set Charge Particle Color
	ChargeFX->Activate();  // Activate the charge particle
						   // Get player mesh and attach this object
	Root->AttachToComponent(OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "PowerSocket");

}

void ABlizzard::CastBlizzard()
{
	ChargeFX->Deactivate(); // Deactivate Charge particle

	// TO DO:  Spawn the Blizzard Drop at the enemy's location, passing it the target and player references.

	// Do the Cast effect after a short delay
	//FTimerHandle CastTimer;
	//GetWorldTimerManager().SetTimer(CastTimer, this, &ABlizzard::DoCastEffect, 1.3f, false);
}

void ABlizzard::DoCastEffect()
{
	
}
