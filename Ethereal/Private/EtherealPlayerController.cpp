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
#include "EtherealPlayerController.h"


AEtherealPlayerController::AEtherealPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HealIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/T_Status_Heal.T_Status_Heal'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RaiseIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/raise.raise'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RegenIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/regen.regen'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RefreshIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/refresh.refresh'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> BarrierIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/protect.protect'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> BerserkIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/berserk.berserk'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HasteIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/haste.haste'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> PoisonIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/T_Status_Poison.T_Status_Poison'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> BurnIconObject(TEXT("Texture2D'/Game/CombatText/UI/Textures/T_Status_Fire.T_Status_Fire'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HealEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Heal.P_Heal'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RaiseEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Heal.P_Heal'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RegenEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Heal.P_Heal'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RefreshEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Heal.P_Heal'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BarrierEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/Barrier-ProtectShield.Barrier-ProtectShield'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BerserkEffectObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_YoYo_Fire_Charge_00.P_YoYo_Fire_Charge_00'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HasteEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Haste1.P_Haste1'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PoisonEffectObject(TEXT("ParticleSystem'/Game/CombatText/Effects/P_Poison.P_Poison'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BurnEffectObject(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	FCombatEffectStruct HealStatus;
	HealStatus.Name = FName(TEXT("HEAL"));
	HealStatus.Text = FText::FromString(FString(TEXT("HEAL")));
	HealStatus.Icon = HealIconObject.Object;
	HealStatus.Effect = HealEffectObject.Object;
	HealStatus.Duration = 2.0f;

	FCombatEffectStruct RaiseStatus;
	RaiseStatus.Name = FName(TEXT("RERAISE"));
	RaiseStatus.Text = FText::FromString(FString(TEXT("RERAISE")));
	RaiseStatus.Icon = RaiseIconObject.Object;
	RaiseStatus.Effect = RaiseEffectObject.Object;
	RaiseStatus.Duration = 0.0f;

	FCombatEffectStruct RegenStatus;
	RegenStatus.Name = FName(TEXT("REGEN"));
	RegenStatus.Text = FText::FromString(FString(TEXT("REGEN")));
	RegenStatus.Icon = RegenIconObject.Object;
	RegenStatus.Effect = RegenEffectObject.Object;
	RegenStatus.Duration = 60.0f;

	FCombatEffectStruct RefreshStatus;
	RefreshStatus.Name = FName(TEXT("REFRESH"));
	RefreshStatus.Text = FText::FromString(FString(TEXT("REFRESH")));
	RefreshStatus.Icon = RefreshIconObject.Object;
	RefreshStatus.Effect = RefreshEffectObject.Object;
	RefreshStatus.Duration = 60.0f;

	FCombatEffectStruct BarrierStatus;
	BarrierStatus.Name = FName(TEXT("PROTECT"));
	BarrierStatus.Text = FText::FromString(FString(TEXT("PROTECT")));
	BarrierStatus.Icon = BarrierIconObject.Object;
	BarrierStatus.Effect = BarrierEffectObject.Object;
	BarrierStatus.Duration = 0.0f;

	FCombatEffectStruct BerserkStatus;
	BerserkStatus.Name = FName(TEXT("BERSERK"));
	BerserkStatus.Text = FText::FromString(FString(TEXT("BERSERK")));
	BerserkStatus.Icon = BerserkIconObject.Object;
	BerserkStatus.Effect = BerserkEffectObject.Object;
	BerserkStatus.Duration = 60.0f;

	FCombatEffectStruct HasteStatus;
	HasteStatus.Name = FName(TEXT("HASTE"));
	HasteStatus.Text = FText::FromString(FString(TEXT("HASTE")));
	HasteStatus.Icon = HasteIconObject.Object;
	HasteStatus.Effect = HasteEffectObject.Object;
	HasteStatus.Duration = 60.0f;

	FCombatEffectStruct PoisonStatus;
	PoisonStatus.Name = FName(TEXT("POISON"));
	PoisonStatus.Text = FText::FromString(FString(TEXT("POISON")));
	PoisonStatus.Icon = PoisonIconObject.Object;
	PoisonStatus.Effect = PoisonEffectObject.Object;
	PoisonStatus.Duration = 10.0f;

	FCombatEffectStruct BurnStatus;
	BurnStatus.Name = FName(TEXT("BURN"));
	BurnStatus.Text = FText::FromString(FString(TEXT("BURN")));
	BurnStatus.Icon = BurnIconObject.Object;
	BurnStatus.Effect = BurnEffectObject.Object;
	BurnStatus.Duration = 10.0f;

	//////////////////////////
	// Complete Status Effect Array
	StatusEffects.Add(HealStatus);
	StatusEffects.Add(RaiseStatus);
	StatusEffects.Add(RegenStatus);
	StatusEffects.Add(RefreshStatus);
	StatusEffects.Add(BarrierStatus);
	StatusEffects.Add(BerserkStatus);
	StatusEffects.Add(HasteStatus);
	StatusEffects.Add(PoisonStatus);
	StatusEffects.Add(BurnStatus);
}

void AEtherealPlayerController::ActivateStatus_Heal_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Reraise_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Regen_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Refresh_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Barrier_Implementation()
{

}

void AEtherealPlayerController::RemoveStatus_Barrier_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Berserk_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Haste_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Poison_Implementation()
{

}

void AEtherealPlayerController::ActivateStatus_Burn_Implementation()
{

}