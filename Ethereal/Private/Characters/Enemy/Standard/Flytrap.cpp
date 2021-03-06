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
#include "Kismet/KismetMathLibrary.h"
#include "Flytrap.h"

AFlytrap::AFlytrap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/EtherealParty/Elongata/Elongata.Elongata'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/EtherealParty/Elongata/Flytrap_AnimBP.Flytrap_AnimBP'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FernBaseMesh(TEXT("StaticMesh'/Game/KiteDemo/Environments/Foliage/Ferns/SM_Fern_01.SM_Fern_01'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> AggroAudioObject(TEXT("SoundCue'/Game/Audio/Party/Flytrap_Aggro_Cue.Flytrap_Aggro_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> HitAudioObject(TEXT("SoundCue'/Game/Audio/Party/Flytrap_Hit_Cue.Flytrap_Hit_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> BiteAudioObject(TEXT("SoundCue'/Game/Audio/Party/Flytrap_Bite_Cue.Flytrap_Bite_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SpitAudioObject(TEXT("SoundCue'/Game/Audio/Party/Flytrap_Spit_Cue.Flytrap_Spit_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathAudioObject(TEXT("SoundCue'/Game/Audio/Party/Flytrap_Death_Cue.Flytrap_Death_Cue'"));

	// Set Default Objects
	GetMesh()->SkeletalMesh = EnemyMesh.Object;
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	SM_FernBase = FernBaseMesh.Object;
	S_AggroAudio = AggroAudioObject.Object;
	S_HitAudio = HitAudioObject.Object;
	S_BiteAudio = BiteAudioObject.Object;
	S_SpitAudio = SpitAudioObject.Object;
	S_DeathAudio = DeathAudioObject.Object;

	// Default Config
	Name = EEnemyNames::EN_Flytrap;
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
	FernBase = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FernBase"));
	FernBase->StaticMesh = SM_FernBase;
	
	AggroAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AggroAudio"));
	AggroAudio->Sound = S_AggroAudio;
	AggroAudio->bAutoActivate = false;
	AggroAudio->SetupAttachment(RootComponent);
	
	HitAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("HitAudio"));
	HitAudio->Sound = S_HitAudio;
	HitAudio->bAutoActivate = false;
	HitAudio->SetupAttachment(RootComponent);
	
	BiteAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("BiteAudio"));
	BiteAudio->Sound = S_BiteAudio;
	BiteAudio->bAutoActivate = false;
	BiteAudio->SetupAttachment(RootComponent);
	
	SpitAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("SpitAudio"));
	SpitAudio->Sound = S_SpitAudio;
	SpitAudio->bAutoActivate = false;
	SpitAudio->SetupAttachment(RootComponent);
	
	DeathAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DeathAudio"));
	DeathAudio->Sound = S_DeathAudio;
	DeathAudio->bAutoActivate = false;
	DeathAudio->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlytrap::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnHearNoise.AddDynamic(this, &AFlytrap::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &AFlytrap::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &AFlytrap::Death); // bind the death fuction to the OnDeath event 
	OnReachedTarget.AddDynamic(this, &AFlytrap::MeleeAttack);  // bind the attack function to the OnReachedTarget event
}

// Melee Attack function
void AFlytrap::MeleeAttack()
{
	EnemyDealDamage(25);
}

void AFlytrap::Death()
{
	IsDead = true;
	DeathAudio->Play();
}

void AFlytrap::LookAtTarget()
{
	FVector FlytrapLocation = this->GetActorLocation(); // GetFlytrap rotation
	FVector PlayerLocation = Target->GetActorLocation();  // get player location
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(FlytrapLocation, PlayerLocation);  // find look at rotation
	FRotator CorrectedRot = FRotator(LookAtRot.Pitch, LookAtRot.Yaw - 90, LookAtRot.Roll);  // correct rotation
	this->SetActorRotation(CorrectedRot); // set rotation to the corrected rotation
}

// A.I. Hearing
void AFlytrap::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TO DO: game-specific logic    
}

// A.I. Sight
void AFlytrap::OnSeePawn(APawn* Pawn)
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