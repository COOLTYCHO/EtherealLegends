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
#include "EtherealPlayerMaster.h"

// Sets default values
AEtherealPlayerMaster::AEtherealPlayerMaster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObject(TEXT("SkeletalMesh'/Game/EtherealParty/Apprentice/Erika_Archer.Erika_Archer'"));
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/EtherealParty/Apprentice/Mixamo_Skeleton_Erika_Anim.Mixamo_Skeleton_Erika_Anim'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ExclamationMesh(TEXT("StaticMesh'/Game/EtherealParty/Apprentice/exclamation.exclamation'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> LevelUpParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/P_levelUp_Eth.P_levelUp_Eth'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> LevelUpCamShakeObject(TEXT("Blueprint'/Game/Blueprints/CamShakes/CS_LevelUp.CS_LevelUp'"));

	SM_Exclamation = ExclamationMesh.Object;
	P_LevelUpFX = LevelUpParticleObject.Object;

	LevelUpCamShake = LevelUpCamShakeObject.Object->GetClass()->StaticClass(); 

	// For whatever reason, uncommenting this Mesh code prevents the editor from loading past 73%... 
	// The same code works for all Enemies, so I can only assume there is an issue with the character's Anim BP.
	//GetMesh()->SkeletalMesh = SkeletalMeshObject.Object;
	//GetMesh()->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());

	// configure the NPC exclamation
	Exclamation = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Exclamation"));
	Exclamation->SetupAttachment(GetMesh());
	Exclamation->StaticMesh = SM_Exclamation;
	Exclamation->SetVisibility(false);
	Exclamation->SetRelativeLocation(FVector(0, 0, 220.0f));
	Exclamation->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	UCommonLibrary::SetupSMComponentsWithCollision(Exclamation);

	MeleeRadius->SetRelativeLocation(FVector(100, 0, 0));
	MeleeRadius->SetSphereRadius(150);

	// configure the targeting reticle
	TargetingReticle->SetRelativeLocation(FVector(0, 0, 200.0f));
	TargetingReticle->SetRelativeRotation(FRotator(0, 0, 180));
	TargetingReticle->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	TargetingReticle->SetVisibility(false);

	LevelUpFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("LevelUpFX"));
	LevelUpFX->SetupAttachment(GetMesh());
	LevelUpFX->Template = P_LevelUpFX;
	LevelUpFX->SetRelativeLocation(FVector(0, 0, 30));
	LevelUpFX->SetRelativeRotation(FRotator(0, 90, 0));
	LevelUpFX->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));
	LevelUpFX->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AEtherealPlayerMaster::BeginPlay()
{
	Super::BeginPlay();

	// Gets and stores a reference to the Player State
	EtherealPlayerState = Cast<AEtherealPlayerState>(GetController()->PlayerState);
	EtherealGameInstance = Cast<UEtherealGameInstance>(GetGameInstance());
	EtherealPlayerController = Cast<AEtherealPlayerController>(GetController());

	if (EtherealPlayerState)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, "EtherealPlayerState reference was successfully initialized from code.");
		EtherealPlayerState->Player = this;  // Sets a reference to itself inside the PlayerState
		EtherealPlayerState->Regen();  // start the regen tick
		EtherealPlayerState->Refresh();  // start the refresh tick
	}

	if (EtherealGameInstance)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, "EtherealGameInstance reference was successfully initialized from code.");
	}
}

// Stops the player's movement
void AEtherealPlayerMaster::StopMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
	SpeedForward = 0;
	SpeedRight = 0;
}

// Toggle Run State
void AEtherealPlayerMaster::ToggleRunState()
{
	if (IsRunning) // player is currently running, so set to walk speed
	{
		IsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 50;
	}
	else // player is currently walking, so set to run speed
	{
		IsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = 100;
	}
}

// This function sets the DamageOutput variable, based on the BaseAtk value of an attack. Returns the ultimate value of damage dealt to an enemy.
void AEtherealPlayerMaster::PlayerDealDamage(float BaseAtk)
{
	// The next three lines are pure Black Magic
	float mod1 = ((BaseAtk + EtherealPlayerState->ATK) / 32);
	float mod2 = ((BaseAtk * EtherealPlayerState->ATK) / 32);
	float mod3 = (((mod1 * mod2) + EtherealPlayerState->ATK) * 40);
	DamageOutput = mod3;  // Set Damage Output
}

// This function deals damage to the player, based on a DamageTaken value supplied by an enemy. This function is usually called by the enemy itself.
void AEtherealPlayerMaster::PlayerTakeDamage(float DamageTaken)
{
	if (!IsDead)  // take no damage if you're already dead!
	{
		IsHit = true;

		// The next four lines are pure Black Magic
		float critical = FMath::FRandRange(1, 1.5f);
		float mod1 = (critical * (EtherealPlayerState->DEF - 512) * DamageTaken);
		float mod2 = (mod1 / (16 * 512));
		float mod3 = FMath::Abs(mod2);
		EtherealPlayerState->HP_Current = (EtherealPlayerState->HP_Current - mod3);  // sets the new HP value

		//FString message = TEXT("Player took damage: ") + UCommonLibrary::GetFloatAsStringWithPrecision(mod3, 0, false) + TEXT("HP: ") + EtherealPlayerState->HP_Current;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		if (critical > 1.4f)
		{
			// Display Critical Damage
			CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_CritDmg, UCommonLibrary::GetFloatAsTextWithPrecision(mod3, 0, false));
		}
		else
		{
			// Display Normal Damage
			CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_Damage, UCommonLibrary::GetFloatAsTextWithPrecision(mod3, 0, false));
		}

		EtherealPlayerState->ForceHPCaps();

	}
}

void AEtherealPlayerMaster::StartDeathCycle_Implementation()
{

}

// Interact with a NPC
void AEtherealPlayerMaster::Interact()
{
	InteractTarget->Interact();  // Interact with the NPC
	Exclamation->SetVisibility(false); // disables the exclamation notification, because you obviously are aware of the NPC
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); // disable player movement
	DoInteractAnim = true;  // Play interact animation
}