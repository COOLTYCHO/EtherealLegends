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
#include "FrostGiant.h"

AFrostGiant::AFrostGiant(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Frost_Giant/SK_Enemy_FrostGiant_Magic.SK_Enemy_FrostGiant_Magic'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/InfinityBladeAdversaries/Enemy/Enemy_Frost_Giant/Anim_FrostGiant.Anim_FrostGiant'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> AggroAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Aggro_Cue.FrostGiant_Aggro_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> LobStartAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_LobStart_Cue.FrostGiant_LobStart_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FirstDropAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_FirstDrop_Cue.FrostGiant_Death_FirstDrop_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SecondDropAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_SecondDrop_Cue.FrostGiant_Death_SecondDrop_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_Cue.FrostGiant_Death_Cue'"));

	// Set Default Objects
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	S_AggroAudio = AggroAudioObject.Object;
	S_LobStartAudio = LobStartAudioObject.Object;
	S_FirstDropAudio = FirstDropAudioObject.Object;
	S_SecondDropAudio = SecondDropAudioObject.Object;
	S_DeathAudio = DeathAudioObject.Object;


	// Default Config
	Name = EEnemyNames::EN_FrostGiant;
	Realm = ERealms::R_Boreal;
	BattleType = EBattleTypes::BT_Standard;
	CommonDrop = EMasterGearList::GL_None;
	UncommonDrop = EMasterGearList::GL_Potion;
	RareDrop = EMasterGearList::GL_Ether;
	AttackDelay = 2.0f;
	BaseEyeHeight = 16;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GetCharacterMovement()->MaxAcceleration = 30;

	DoLob = false;
	DoMelee = false;

	// Pawn A.I. config
	PawnSensing->HearingThreshold = 150;
	PawnSensing->LOSHearingThreshold = 200;
	PawnSensing->SightRadius = 250;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	AcceptanceRadius = 75.0f;
	RunAI = false;

	// Mesh Config
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Melee Radius Config
	MeleeRadius->SetSphereRadius(100);
	MeleeRadius->SetRelativeLocation(FVector(25, 0, 50));

	// Targeting Reticle config
	TargetingReticle->SetRelativeLocation(FVector(0, 0, 400));
	TargetingReticle->SetRelativeRotation(FRotator(0, 0, 180));
	TargetingReticle->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	
	// FX Config
	HitFX->SetRelativeLocation(FVector(0, 0, 10));

	DeathFX->SetRelativeLocation(FVector(0, 0, -90));
	DeathFX->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	
	DisappearFX->SetRelativeLocation(FVector(0, 0, -20));
	DisappearFX->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));

	// Enemy-Specific Object Config
	AggroAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AggroAudio"));
	AggroAudio->Sound = S_AggroAudio;
	AggroAudio->bAutoActivate = false;
	AggroAudio->SetupAttachment(RootComponent);
	
	LobStartAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("LobStartAudio"));
	LobStartAudio->Sound = S_LobStartAudio;
	LobStartAudio->bAutoActivate = false;
	LobStartAudio->SetupAttachment(RootComponent);
	
	FirstDropAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("FirstDropAudio"));
	FirstDropAudio->Sound = S_FirstDropAudio;
	FirstDropAudio->bAutoActivate = false;
	FirstDropAudio->SetupAttachment(RootComponent);
	
	SecondDropAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("SecondDropAudio"));
	SecondDropAudio->Sound = S_SecondDropAudio;
	SecondDropAudio->bAutoActivate = false;
	SecondDropAudio->SetupAttachment(RootComponent);
	
	DeathAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DeathAudio"));
	DeathAudio->Sound = S_DeathAudio;
	DeathAudio->bAutoActivate = false;
	DeathAudio->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFrostGiant::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnHearNoise.AddDynamic(this, &AFrostGiant::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &AFrostGiant::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &AFrostGiant::Death); // bind the death fuction to the OnDeath event 
	OnReachedTarget.AddDynamic(this, &AFrostGiant::MeleeAttack);  // bind the attack function to the OnReachedTarget event
}

// Melee Attack function
void AFrostGiant::MeleeAttack()
{
	EnemyDealDamage(25);
}

void AFrostGiant::Death()
{
	IsDead = true;
}

// A.I. Hearing
void AFrostGiant::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TO DO: game-specific logic    
}

// A.I. Sight
void AFrostGiant::OnSeePawn(APawn* Pawn)
{
	if (!IsDead)
	{
		if (!IsAggroed)
		{
			Aggro(Pawn);
			RunToTarget();
		}
	}	
}
