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
#include "SoulEater.h"

// Sets default values
ASoulEater::ASoulEater(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/EtherealParty/SoulEater/SoulEater.SoulEater'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/EtherealParty/SoulEater/Anim_SoulEater.Anim_SoulEater'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathAudioObject(TEXT("SoundCue'/Game/Audio/Party/SoulEater_Death_Cue.SoulEater_Death_Cue'"));

	S_DeathAudio = DeathAudioObject.Object;	

	// Default Config
	Name = EEnemyNames::EN_SoulEater;
	Realm = ERealms::R_Shiitake;
	BattleType = EBattleTypes::BT_Standard;
	CommonDrop = EMasterGearList::GL_None;
	UncommonDrop = EMasterGearList::GL_Potion;
	RareDrop = EMasterGearList::GL_Ether;
	AttackDelay = 2.0f;
	BaseEyeHeight = 16;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	GetCharacterMovement()->MaxAcceleration = 30;

	// Pawn A.I. config
	PawnSensing->HearingThreshold = 150;
	PawnSensing->LOSHearingThreshold = 200;
	PawnSensing->SightRadius = 250;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	AcceptanceRadius = 25.0f;
	RunAI = false;

	// Mesh Config
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeScale3D(FVector(2, 2, 2));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Melee Radius Config
	MeleeRadius->SetSphereRadius(100);
	MeleeRadius->SetRelativeLocation(FVector(15, 0, 0));

	// Targeting Reticle config
	TargetingReticle->SetRelativeLocation(FVector(0, 0, 130));
	TargetingReticle->SetRelativeRotation(FRotator(0, 0, 180));
	TargetingReticle->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	
	// Death FX Config
	DeathFX->SetRelativeLocation(FVector(0, 0, -90));
	DeathFX->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	
	DisappearFX->SetRelativeLocation(FVector(0, 0, -20));

	// Enemy-Specific Object Config
	DeathAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DeathAudio"));
	DeathAudio->Sound = S_DeathAudio;
	DeathAudio->bAutoActivate = false;
	DeathAudio->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASoulEater::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnHearNoise.AddDynamic(this, &ASoulEater::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &ASoulEater::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &ASoulEater::Death); // bind the death fuction to the OnDeath event 
	OnReachedTarget.AddDynamic(this, &ASoulEater::MeleeAttack);  // bind the attack function to the OnReachedTarget event
}

void ASoulEater::MeleeAttack()
{
	EnemyDealDamage(25);
	Attack = true;
}

void ASoulEater::Death()
{
	DeathAudio->Play();  // Play death audio
	Target->EtherealPlayerState->EnemyKillReward(Level, CommonDrop, UncommonDrop, RareDrop);  // reward the player for killing this enemy
}

void ASoulEater::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TO DO: game-specific logic    
}

void ASoulEater::OnSeePawn(APawn* Pawn)
{
	if (!IsDead)
	{
		if (!IsAggroed)
		{
			Aggro(Pawn);
			Roar = true;

			//RunToTarget() after the Roar. This timer is not needed because this function was tied to an anim notify in Roar
			//FTimerHandle DestroyTimer;
			//GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEtherealEnemyMaster::RunToTarget, 3.0f, false);	
		}	
	}
}