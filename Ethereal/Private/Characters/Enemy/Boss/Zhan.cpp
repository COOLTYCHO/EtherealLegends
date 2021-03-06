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
#include "Zhan.h"

AZhan::AZhan(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/MixamoAnimPack/Mixamo_Ganfault/Mesh/Mixamo_Ganfault_Aure.Mixamo_Ganfault_Aure'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/MixamoAnimPack/Mixamo_Ganfault/Anims/Zhan_Anim.Zhan_Anim'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BladeMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword10/SK_Blade_HeroSword10.SK_Blade_HeroSword10'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EyeParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Chicken/Souleater_Statue_EyeIdle.Souleater_Statue_EyeIdle'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BuildUpParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/Fire/P_Beam_Laser_Fire_BuildUP.P_Beam_Laser_Fire_BuildUP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PowerBlastParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/Fire/P_Beam_Laser_Fire.P_Beam_Laser_Fire'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpinAtkParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_TeleCharge/Zhan_SpinAtkFX.Zhan_SpinAtkFX'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> PowerBlastAudioObject(TEXT("SoundCue'/Game/Audio/Party/Zhan_PowerBlast_Cue.Zhan_PowerBlast_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SpinAtkAudioObject(TEXT("SoundCue'/Game/Audio/Party/Zhan_SpinAtk_Cue.Zhan_SpinAtk_Cue'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> LobBlueprintObject(TEXT("Blueprint'/Game/Blueprints/Characters/Enemy/6-CelestialNexus/Zhan_AggroDrop.Zhan_AggroDrop'"));

	// Set Default Objects
	SK_Blade = BladeMesh.Object;
	P_EyeFX = EyeParticleObject.Object;
	P_BuildUpFX = BuildUpParticleObject.Object;
	P_PowerBlastFX = PowerBlastParticleObject.Object;
	P_SpinAtkFX = SpinAtkParticleObject.Object;
	S_PowerBlastAudio = PowerBlastAudioObject.Object;
	S_SpinAtkAudio = SpinAtkAudioObject.Object;

	/////////////////////////////////////
	// DEFAULTS

	Name = EEnemyNames::EN_Zhan;
	Realm = ERealms::R_Celestial;
	BattleType = EBattleTypes::BT_Boss;
	CommonDrop = EMasterGearList::GL_Reraise;
	UncommonDrop = EMasterGearList::GL_Elixer;
	RareDrop = EMasterGearList::GL_Comet;
	AttackDelay = 2.5f;
	BaseEyeHeight = 16;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	GetCharacterMovement()->MaxAcceleration = 0;	

	AggroDropBP = (UClass*)LobBlueprintObject.Object->GeneratedClass;

	// A.I. Config
	PawnSensing->HearingThreshold = 300;
	PawnSensing->LOSHearingThreshold = 400;
	PawnSensing->SightRadius = 750;
	PawnSensing->SetPeripheralVisionAngle(50.0f);
	AcceptanceRadius = 750.0f;
	RunAI = false;

	// Mesh Config
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeScale3D(FVector(1, 1, 1));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// Melee Radius Config
	MeleeRadius->SetSphereRadius(300);
	MeleeRadius->SetRelativeLocation(FVector(0, 0, 0));

	// Targeting Reticle config
	TargetingReticle->SetRelativeLocation(FVector(0, 0, 180));
	TargetingReticle->SetRelativeRotation(FRotator(0, 0, 180));
	TargetingReticle->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	// Hit & Death Effect Config
	HitFX->SetRelativeLocation(FVector(40, 0, 10));
	DeathFX->SetRelativeLocation(FVector(0, 0, -90));
	DeathFX->SetRelativeScale3D(FVector(1, 1, 1));
	DisappearFX->SetRelativeLocation(FVector(0, 0, -20));

	// Power Blast Box
	PowerBlastBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("PowerBlastBox"));
	PowerBlastBox->SetupAttachment(RootComponent);
	PowerBlastBox->SetRelativeLocation(FVector(600, 0, 0));
	PowerBlastBox->SetBoxExtent(FVector(600, 60, 60));

	// Spin Attack Box
	SpinAtkBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("SpinAtkBox"));
	SpinAtkBox->SetupAttachment(RootComponent);
	SpinAtkBox->SetBoxExtent(FVector(300, 80, 80));

	// Blade skeletal mesh
	Blade = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Blade"));
	Blade->SetupAttachment(GetMesh(), FName(TEXT("R_Weapon")));
	Blade->SkeletalMesh = SK_Blade;

	// Glowing Eye Particle Effect
	EyeFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("EyeFX"));
	EyeFX->SetupAttachment(GetMesh(), FName(TEXT("EyeSocket")));
	EyeFX->Template = P_EyeFX;
	EyeFX->bAutoActivate = false;

	// Build Up Effect (for him teleporting away)
	BuildUpFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("BuildUpFX"));
	BuildUpFX->SetupAttachment(RootComponent);
	BuildUpFX->Template = P_BuildUpFX;
	BuildUpFX->bAutoActivate = false;

	// Power Blast attack effect
	PowerBlastFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("PowerBlastFX"));
	PowerBlastFX->SetupAttachment(RootComponent);
	PowerBlastFX->Template = P_PowerBlastFX;
	PowerBlastFX->bAutoActivate = false;

	// Spin Attack effect
	SpinAtkFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("SpinAtkFX"));
	SpinAtkFX->SetupAttachment(RootComponent);
	SpinAtkFX->Template = P_SpinAtkFX;
	SpinAtkFX->bAutoActivate = false;

	// Power Blast attack audio
	PowerBlastAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("SecondDropAudio"));
	PowerBlastAudio->SetupAttachment(RootComponent);
	PowerBlastAudio->Sound = S_PowerBlastAudio;
	PowerBlastAudio->bAutoActivate = false;

	// Spin Attack audio
	SpinAtkAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DeathAudio"));
	SpinAtkAudio->SetupAttachment(RootComponent);
	SpinAtkAudio->Sound = S_SpinAtkAudio;
	SpinAtkAudio->bAutoActivate = false;	

	///////////////////////////////////////////
	// STARTING HEIGHT MOD

	StartHeightOffset = 200.0f;
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, (GetActorLocation().Z + StartHeightOffset)));  // places Zhan in the air, so he can burst out all awesome during his aggro sequence
	GetCharacterMovement()->GravityScale = 0; // turn off Gravity so he stays in the air
}

// Called when the game starts or when spawned
void AZhan::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetVisibility(false, true); // hide root object and propagate to all children

	PawnSensing->OnHearNoise.AddDynamic(this, &AZhan::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &AZhan::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &AZhan::Death); // bind the death fuction to the OnDeath event 
	//OnReachedTarget.AddDynamic(this, &AZhan::AttackCycle);  // bind the attack function to the OnReachedTarget event
}

void AZhan::InitAggro()
{
	Aggro(Target);
	// Spawn Zhan's Aggro Drop at current location - StartHeightOffset on Z
	AActor* AggroDrop = UCommonLibrary::SpawnBP(GetWorld(), AggroDropBP, FVector(GetActorLocation().X, GetActorLocation().Y, (GetActorLocation().Z - StartHeightOffset)), GetActorRotation());
	AudioManager->Play_Zhan_Intro();
	// Start the fall process
	FTimerHandle AggroTimer;
	GetWorldTimerManager().SetTimer(AggroTimer, this, &AZhan::FallToAggro, 9.8f, false);
}

void AZhan::FallToAggro()
{
	this->GetCharacterMovement()->GravityScale = 0.1f;  // make him fall in slow motion
	this->GetCapsuleComponent()->SetVisibility(true, true);  // this will also turn on the Targeting Reticle
	DoFallAggro = true;

	UGameplayStatics::PlayWorldCameraShake(GetWorld(), Target->LevelUpCamShake, Target->GetActorLocation(), 0, 10000, 1, false);  // level up cam shake 
	// TO DO : Client Play Force Feedback FF_ZhanSpawn

	// earlier code sets the reticle visible, be sure to turn it off.
	if (TargetingReticle->IsVisible())
	{
		TargetingReticle->SetVisibility(false);
	}

	// Start the attack cycle
	FTimerHandle AttackTimer;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AZhan::StartAttacking, 7.0f, false);
}

void AZhan::StartAttacking()
{
	AudioManager->Play_Zhan_Battle();
	GetCharacterMovement()->GravityScale = 1.0f;
	RunToTarget();
}

void AZhan::Death()
{
	IsDead = true;
}

void AZhan::PowerBlast()
{
	DoPowerBlast = true;
	EnemyDealDamage(30);
	PowerBlastAudio->Play();
	// TO DO : Delay 0.5
	// Check power blast collider and have player take damage
	// wait attack delay, then RunToTarget()
}

void AZhan::SpinAttack()
{
	DoSpinAtk = true;
	EnemyDealDamage(25);
	SpinAtkAudio->Play();
	// TO DO : delay 0.5
	// Check SpinAtk collider and have the player take damage
	// wait attack delay and RunToTarget()
}

void AZhan::RadialBlast()
{
	DoRadialBlast = true;
	EnemyDealDamage(40);
	// TO DO : Spawn Zhan Radial Blast Drop
	// TO DO : DELAY 1.5
	// TO DO : Call FireRadialBlast() on the drop, passing Zhan's damage output
	// wait attack delay and call RunToTarget()
}

void AZhan::Escape()
{
	// Spawn the Escape Portal
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EscapePortalObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Ausar/Zhan_EscapePortal.Zhan_EscapePortal'"));
	UParticleSystem* P_EscapePortal = EscapePortalObject.Object;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_EscapePortal, this->GetActorLocation(), this->GetActorRotation(), true);  

	// Add the Escape Audio Components
	static ConstructorHelpers::FObjectFinder<USoundCue> PortalExplosionAudioObject(TEXT("SoundCue'/Game/Audio/Impacts/Zhan_PortalExplosion_Cue.Zhan_PortalExplosion_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> ZhanEscapeAudioObject(TEXT("SoundCue'/Game/Audio/Impacts/Zhan_Escape_Cue.Zhan_Escape_Cue'"));
	USoundCue* S_PortalExplosion = PortalExplosionAudioObject.Object;
	USoundCue* S_ZhanEscape = ZhanEscapeAudioObject.Object;
	UGameplayStatics::PlaySound2D(GetWorld(), S_PortalExplosion);
	UGameplayStatics::PlaySound2D(GetWorld(), S_ZhanEscape);

	// TO DO :  4 second timeline, slowly rising Zhan into the air. Timeline simply adds 0.5 to his Z every update cycle.
	// TO DO :  when timeline is complete, AudioManager->FadeOut(), then spawn ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/WeaponCombo/P_Cube_Mesh_Test.P_Cube_Mesh_Test'
	this->GetCapsuleComponent()->SetVisibility(false, true); // hide root object and propagate to all children
	// DELAY 1.2 seconds
	// destroy cube mesh particle that was spawned earlier
	// AudioManager->PlayLevelUpFanfare()
	// TO DO : Spawn the Arcadia Return Portal wherever Zhan died
	// DELAY 5 seconds
	// AudioManager->PlayBGM()
}

void AZhan::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TO DO: game-specific logic    
}

void AZhan::OnSeePawn(APawn* Pawn)
{
	if (!IsAggroed)
	{
		Aggro(Pawn);
	}
}