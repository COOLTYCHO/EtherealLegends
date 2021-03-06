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
#include "EarthTroll.h"

AEarthTroll::AEarthTroll(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/SK_Troll_Poison.SK_Troll_Poison'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Anim_Troll.Anim_Troll'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HammerMesh(TEXT("StaticMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Meshes/SM_Troll_hammer.SM_Troll_hammer'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VomitParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Troll/Poison/P_Troll_Poison_Vomit_01.P_Troll_Poison_Vomit_01'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> AggroAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Aggro_Cue.FrostGiant_Aggro_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> HitAudioObject(TEXT("SoundCue'/Game/EtherealParty/IceKnight/Audio/Avatar_IceKnight_Hit_Cue.Avatar_IceKnight_Hit_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SwipeAudioObject(TEXT("SoundCue'/Game/Sounds/FoleyBodyMoves/Stereo/FoleyBody_HandSwipe_Stereo_Cue.FoleyBody_HandSwipe_Stereo_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SpitAudioObject(TEXT("SoundCue'/Game/Audio/Party/GiantSpider_AtkSpit_Cue.GiantSpider_AtkSpit_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> RoarAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostCaptain_StompRoar_Cue.FrostCaptain_StompRoar_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FirstDropAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_FirstDrop_Cue.FrostGiant_Death_FirstDrop_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SecondDropAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_SecondDrop_Cue.FrostGiant_Death_SecondDrop_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathAudioObject(TEXT("SoundCue'/Game/Audio/Party/FrostGiant_Death_Cue.FrostGiant_Death_Cue'"));

	// Set Default Objects
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	SM_Hammer = HammerMesh.Object;
	P_VomitFX = VomitParticleObject.Object;
	S_AggroAudio = AggroAudioObject.Object;
	S_HitAudio = HitAudioObject.Object;
	S_SwipeAudio = SwipeAudioObject.Object;
	S_SpitAudio = SpitAudioObject.Object;
	S_RoarAudio = RoarAudioObject.Object;
	S_FirstDropAudio = FirstDropAudioObject.Object;
	S_SecondDropAudio = SecondDropAudioObject.Object;
	S_DeathAudio = DeathAudioObject.Object;

	// Default Config
	Name = EEnemyNames::EN_EarthTroll;
	Realm = ERealms::R_Yggdrasil;
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
	SpitBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("SpitBox"));
	SpitBox->SetRelativeLocation(FVector(0, 350, 90));
	SpitBox->SetBoxExtent(FVector(100, 400, 150));
	SpitBox->SetupAttachment(RootComponent);
	
	Hammer = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Hammer"));
	Hammer->StaticMesh = SM_Hammer;
	Hammer->SetupAttachment(GetMesh(), "WeaponSocket");
	
	VomitFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("VomitFX"));
	VomitFX->Template = P_VomitFX;
	VomitFX->bAutoActivate = false;
	VomitFX->SetupAttachment(GetMesh(), "VomitSocket");
	
	AggroAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AggroAudio"));
	AggroAudio->Sound = S_AggroAudio;
	AggroAudio->bAutoActivate = false;
	AggroAudio->SetupAttachment(RootComponent);
	
	HitAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("HitAudio"));
	HitAudio->Sound = S_HitAudio;
	HitAudio->bAutoActivate = false;
	HitAudio->SetupAttachment(RootComponent);
	
	SwipeAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("SwipeAudio"));
	SwipeAudio->Sound = S_SwipeAudio;
	SwipeAudio->bAutoActivate = false;
	SwipeAudio->SetupAttachment(RootComponent);
	
	SpitAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("SpitAudio"));
	SpitAudio->Sound = S_SpitAudio;
	SpitAudio->bAutoActivate = false;
	SpitAudio->SetupAttachment(RootComponent);
	
	RoarAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("RoarAudio"));
	RoarAudio->Sound = S_RoarAudio;
	RoarAudio->bAutoActivate = false;
	RoarAudio->SetupAttachment(RootComponent);
	
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
void AEarthTroll::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnHearNoise.AddDynamic(this, &AEarthTroll::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &AEarthTroll::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &AEarthTroll::Death); // bind the death fuction to the OnDeath event 
	OnReachedTarget.AddDynamic(this, &AEarthTroll::MeleeAttack);  // bind the attack function to the OnReachedTarget event
}

// Melee Attack function
void AEarthTroll::MeleeAttack()
{
	EnemyDealDamage(25);
	
	// TO DO :  attack stuff
}

void AEarthTroll::Death()
{
	IsDead = true;
	DeathAudio->Play();
}

void AEarthTroll::Sling()
{
	EnemyDealDamage(45);
	DoSling = true;
	// TO DO :  Spawn the Troll Sling Drop, pass it the target and damage output
	// Wait attack delay, then call RunToTarget()
}

void AEarthTroll::Swipe()
{
	EnemyDealDamage(40);
	DoSwipe = true;
	// TO DO : Set AoE Cone visible
	// Wait attack delay, then call RunToTarget()
}

void AEarthTroll::Spit()
{
	EnemyDealDamage(50);
	DoSpit = true;
	// Wait attack delay, then call RunToTarget()
}

// A.I. Hearing
void AEarthTroll::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TO DO: game-specific logic    
}

// A.I. Sight
void AEarthTroll::OnSeePawn(APawn* Pawn)
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
