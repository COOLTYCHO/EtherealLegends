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

// These are all included here so that they can be referenced for the CreateGear() function. It's UGLY AS SHIT, and should probably be refactored.
#include "Gear/Magic/Spells/Cure.h"
#include "Gear/Magic/Spells/CureII.h"
#include "Gear/Magic/Spells/Regen.h"
#include "Gear/Magic/Spells/Refresh.h"
#include "Gear/Magic/Spells/Blizzard.h"
#include "Gear/Magic/Spells/Thunder.h"
#include "Gear/Magic/Spells/Fire.h"
#include "Gear/Magic/Spells/Berserk.h"
#include "Gear/Magic/Spells/Barrier.h"
#include "Gear/Magic/Spells/Haste.h"
#include "Gear/Magic/Spells/Dash.h"
#include "Gear/Weapons/OneHanded/IronSword.h"
#include "Gear/Weapons/OneHanded/Joyeuse.h"
#include "Gear/Weapons/OneHanded/Hauteclaire.h"
#include "Gear/Weapons/OneHanded/Excalibur.h"
#include "Gear/Weapons/OneHanded/Shields/SpartanShield.h"
#include "Gear/Weapons/OneHanded/Shields/WarAspis.h"
#include "Gear/Weapons/Ranged/ShortBow.h"
#include "Gear/Weapons/Ranged/EagleEye.h"
#include "Gear/Weapons/Ranged/EurytosBow.h"
#include "Gear/Weapons/Ranged/Annihilator.h"
#include "Gear/Weapons/Ranged/Ammo/DemonArrow.h"
#include "Gear/Weapons/Ranged/Ammo/WoodenArrow.h"
#include "Gear/Weapons/TwoHanded/Marauder.h"
#include "Gear/Weapons/TwoHanded/Juggernaut.h"
#include "Gear/Weapons/TwoHanded/Claymore.h"
#include "Gear/Weapons/TwoHanded/Marauder.h"
#include "Gear/Weapons/TwoHanded/Temperance.h"
#include "Gear/Weapons/Casting/MagicOrbs.h"
#include "Gear/Items/Consumable/Potion.h"
#include "Gear/Items/Consumable/HiPotion.h"
#include "Gear/Items/Consumable/Ether.h"
#include "Gear/Items/Consumable/HiEther.h"
#include "Gear/Items/Consumable/Reraise.h"
#include "Gear/Items/Consumable/Elixer.h"
#include "Gear/Items/Consumable/Adrenaline.h"
#include "Gear/Items/Consumable/SentinelBrew.h"

#include "Gear/Items/Item_Master.h"
#include "Characters/Player/EtherealPlayerMaster.h"
#include "Characters/Enemy/EtherealEnemyMaster.h"
#include "EtherealPlayerState.h"

AEtherealPlayerState::AEtherealPlayerState(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	// Set Default Stats
	PlayerLevel = 5;
	HP_Current = 0;
	HP_Max = 0;
	MP_Current = 0;
	MP_Max = 0;
	TP_Current = 0;
	TP_Max = 0;
	ATK = 0;
	DEF = 0;
	SPD = 0;
	RefreshRate = 0;
	RegenRate = 0;
	EXP_Current = 5000;
	Gold_Current = 0;

	Binding_Magic_Slot1 = EMasterGearList::GL_None;
	Binding_Magic_Slot2 = EMasterGearList::GL_None;
	Binding_Magic_Slot3 = EMasterGearList::GL_None;
	Binding_Magic_Slot4 = EMasterGearList::GL_None;
	Binding_Magic_Slot5 = EMasterGearList::GL_None;
	Binding_Magic_Slot6 = EMasterGearList::GL_None;

	Binding_Weapon_Main = EMasterGearList::GL_None;
	Binding_Weapon_Sub = EMasterGearList::GL_None;

	Binding_Item_Current = EMasterGearList::GL_None;
}

// Called when the game starts or when spawned
void AEtherealPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

// SET BASE STATS
void AEtherealPlayerState::SetBaseStats()
{
	int32 BaseHP = PlayerLevel * 200;
	int32 BaseMP = PlayerLevel * 20;
	float BaseStats = PlayerLevel * 5.1f;

	HP_Current = BaseHP;
	HP_Max = BaseHP;
	MP_Current = BaseMP;
	MP_Max = BaseMP;
	ATK = BaseStats;
	DEF = BaseStats;
	SPD = BaseStats;
	RefreshRate = 1;
	RegenRate = 1;
}

// Completely Refill HP & MP 
void AEtherealPlayerState::RestoreAll()
{
	HP_Current = HP_Max;
	MP_Current = MP_Max;
}

// Force HP Caps
void AEtherealPlayerState::ForceHPCaps()
{
	// If HP is greater than Max, set HP to Max
	if (HP_Current > HP_Max)
	{
		HP_Current = HP_Max;
	}
	// If HP is less than Zero, set HP to Zero
	if (HP_Current < 0)
	{
		HP_Current = 0;
		Player->StartDeathCycle();  // start death cycle
	}
}

// Force MP Caps
void AEtherealPlayerState::ForceMPCaps()
{
	// If MP is greater than Max, set MP to Max
	if (MP_Current > MP_Max)
	{
		MP_Current = MP_Max;
	}
	// if MP is less than Zero, set MP to Zero
	if (MP_Current < 0)
	{
		MP_Current = 0;
	}
}

// SET TNL
void AEtherealPlayerState::SetTNL()
{
	switch (PlayerLevel)
	{
		case 1:
			EXP_ToNextLevel = 1250;
			break;
		case 2:
			EXP_ToNextLevel = 2250;
			break;
		case 3:
			EXP_ToNextLevel = 3500;
			break;
		case 4:
			EXP_ToNextLevel = 5000;
			break;
		case 5:
			EXP_ToNextLevel = 6750;
			break;
		case 6:
			EXP_ToNextLevel = 8750;
			break;
		case 7:
			EXP_ToNextLevel = 10950;
			break;
		case 8:
			EXP_ToNextLevel = 13350;
			break;
		case 9:
			EXP_ToNextLevel = 15950;
			break;
		case 10:
			EXP_ToNextLevel = 13350;
			break;
		case 11:
			EXP_ToNextLevel = 15950;
			break;
		case 12:
			EXP_ToNextLevel = 18750;
			break;
		case 13:
			EXP_ToNextLevel = 21750;
			break;
		case 14:
			EXP_ToNextLevel = 24950;
			break;
		case 15:
			EXP_ToNextLevel = 28350;
			break;
		case 16:
			EXP_ToNextLevel = 31950;
			break;
		case 17:
			EXP_ToNextLevel = 35750;
			break;
		case 18:
			EXP_ToNextLevel = 39750;
			break;
		case 19:
			EXP_ToNextLevel = 43950;
			break;
		case 20:
			EXP_ToNextLevel = 48350;
			break;
		case 21:
			EXP_ToNextLevel = 52950;
			break;
		case 22:
			EXP_ToNextLevel = 57750;
			break;
		case 23:
			EXP_ToNextLevel = 62750;
			break;
		case 24:
			EXP_ToNextLevel = 67850;
			break;
		case 25:
			EXP_ToNextLevel = 73050;
			break;
		case 26:
			EXP_ToNextLevel = 78350;
			break;
		case 27:
			EXP_ToNextLevel = 83750;
			break;
		case 28:
			EXP_ToNextLevel = 89250;
			break;
		case 29:
			EXP_ToNextLevel = 94850;
			break;
		case 30:
			EXP_ToNextLevel = 100550;
			break;
		case 31:
			EXP_ToNextLevel = 106350;
			break;
		case 32:
			EXP_ToNextLevel = 112250;
			break;
		case 33:
			EXP_ToNextLevel = 118250;
			break;
		case 34:
			EXP_ToNextLevel = 124350;
			break;
		case 35:
			EXP_ToNextLevel = 130550;
			break;
		case 36:
			EXP_ToNextLevel = 136850;
			break;
		case 37:
			EXP_ToNextLevel = 143250;
			break;
		case 38:
			EXP_ToNextLevel = 149750;
			break;
		case 39:
			EXP_ToNextLevel = 156350;
			break;
		case 40:
			EXP_ToNextLevel = 163050;
			break;
		case 41:
			EXP_ToNextLevel = 169850;
			break;
		case 42:
			EXP_ToNextLevel = 176750;
			break;
		case 43:
			EXP_ToNextLevel = 183750;
			break;
		case 44:
			EXP_ToNextLevel = 190850;
			break;
		case 45:
			EXP_ToNextLevel = 198050;
			break;
		case 46:
			EXP_ToNextLevel = 205350;
			break;
		case 47:
			EXP_ToNextLevel = 212750;
			break;
		case 48:
			EXP_ToNextLevel = 220250;
			break;
		case 49:
			EXP_ToNextLevel = 227850;
			break;
		case 50:
			EXP_ToNextLevel = 235550;
			break;
	}
}

// Enemy Kill Reward
void AEtherealPlayerState::AddEXP(int modEXP)
{
	FString message = TEXT("+  ") + FString::FromInt(modEXP) + TEXT("  EXP");  // build the proper string
	FText displaytext = FText::FromString(message);  // convert it to text

	EXP_Current = EXP_Current + modEXP;  // add EXP

	Player->CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_Text, displaytext);  // Display EXP gained on screen
}

// Enemy Kill Reward
void AEtherealPlayerState::AddGold(int modGold)
{
	FString message = TEXT("+  ") + FString::FromInt(modGold) + TEXT("  GOLD");  // build the proper string
	FText displaytext = FText::FromString(message);  // convert it to text

	Gold_Current = Gold_Current + modGold;  // add Gold

	Player->CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_Text, displaytext);  // Display Gold gained on screen
}

// Drop Items
void AEtherealPlayerState::DropItems(EMasterGearList Common, EMasterGearList Uncommon, EMasterGearList Rare)
{
	// ENEMY KILL RANDOM DROPS
	int32 Drop = FMath::FloorToInt(FMath::FRandRange(0, 100));

	if (Drop <= 5)
	{
		// RARE DROP @ 5 %
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f); // Slow Motion because you got something cool
		AddToInventory(Rare, false);

		// Display the Item name on screen (this usually only occurs when AddToInventory() is called from an Enemy Drop
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMasterGearList"), true);
		FText DropName = EnumPtr->GetDisplayNameText((int32)Rare);

		Player->CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_Drop, DropName);  // display text on screen
	}
	if (Drop >= 70)
	{
		// UNCOMMON DROP @ 30 %
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f); // Slow Motion because you got something cool
		AddToInventory(Uncommon, false);

		// Display the Item name on screen (this usually only occurs when AddToInventory() is called from an Enemy Drop
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMasterGearList"), true);
		FText DropName = EnumPtr->GetDisplayNameText((int32)Uncommon);

		Player->CombatTextComponent->ShowCombatText(ECombatTextTypes::TT_Drop, DropName);  // display text on screen
	}
	if (Drop > 5 && Drop < 70)
	{
		if (Common != EMasterGearList::GL_None)  // do nothing if common drop = none
		{
			// COMMON DROP @ 65 %
			AddToInventory(Common, false);
		}		
	}
}

// Enemy Kill Reward
void AEtherealPlayerState::EnemyKillReward(int EnemyLevel, EMasterGearList Common, EMasterGearList Uncommon, EMasterGearList Rare)
{
	KillCount++;  // increment total kill count

	// create bonus values
	int32 modEXP = (EnemyLevel * 25);
	int32 modGold = (EnemyLevel * 15);

	// wait a bit before adding EXP (delays the UI display for readability).
	FTimerDelegate DelegateEXP;
	DelegateEXP.BindUFunction(this, FName("AddEXP"), modEXP);
	FTimerHandle EXPTimer;
	GetWorldTimerManager().SetTimer(EXPTimer, DelegateEXP, 0.1f, false);

	// wait a bit longer before adding Gold (delays the UI display, again for readability).
	FTimerDelegate DelegateGold;
	DelegateGold.BindUFunction(this, FName("AddGold"), modGold);
	FTimerHandle GoldTimer;
	GetWorldTimerManager().SetTimer(GoldTimer, DelegateGold, 0.5f, false);

	// wait just a bit longer to determine whether or not the player should level up
	FTimerHandle LevelUpTimer;
	GetWorldTimerManager().SetTimer(LevelUpTimer, this, &AEtherealPlayerState::LevelUp, 0.5f, false);

	// wait a just a bit longer to distribute drops (delays the UI display, again for readability).
	FTimerDelegate DelegateDrops;
	DelegateDrops.BindUFunction(this, FName("DropItems"), Common, Uncommon, Rare);
	FTimerHandle DropTimer;
	GetWorldTimerManager().SetTimer(DropTimer, DelegateDrops, 0.8f, false);
}

// Level Up
void AEtherealPlayerState::LevelUp()
{
	if (EXP_Current >= EXP_ToNextLevel)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Player->LevelUpCamShake, Player->GetActorLocation(), 0, 10000, 1, false);  // level up cam shake 
		PlayerLevel++;  // player level + 1
		SetBaseStats();

		for (AEtherealGearMaster* GearItem : Inventory)  // scan the inventory...
		{
			if (GearItem->IsBound)  // if ItemToGet matches the name of an item currently in the inventory
			{
				AddGearStats(GearItem->Name);  // Add the Gear Stats for each currently bound item
			}
		}

		RestoreAll();  // restore all hp and mp
		Player->LevelUpFX->Activate(true);  // activate level up particle effect
		Player->AudioManager->Play_SFX_LevelUp();  // play level up audio sound effect
		SetTNL();  // set new TNL

		if (Player->AggroList.Num() > 0)
		{
			AEtherealEnemyMaster* EnemyZero = Player->AggroList[0];  // get the enemy at index 0

			// if the aggro list still has enemies, you're still in battle, so play the battle music again
			FTimerDelegate DelegateBattle;
			DelegateBattle.BindUFunction(Player->AudioManager, FName("Play_BattleMusic"), EnemyZero->BattleType);
			FTimerHandle BattleTimer;
			GetWorldTimerManager().SetTimer(BattleTimer, DelegateBattle, 0.8f, false);
		}
		else
		{
			// the aggro list is empty, so you must be out of battle. Play the current Realm's BGM
			FTimerDelegate DelegateBGM;
			DelegateBGM.BindUFunction(Player->AudioManager, FName("Play_BGM"), Player->EtherealGameInstance->CurrentRealm);
			FTimerHandle BGMTimer;
			GetWorldTimerManager().SetTimer(BGMTimer, DelegateBGM, 0.8f, false);
		}
	}
}

// Regen Tick
void AEtherealPlayerState::Regen()
{
	if (!Player->IsDead) // do not tick if Player is dead
	{
		HP_Current = HP_Current + RegenRate;
		ForceHPCaps();
		// Call Regen again after 1 second
		FTimerHandle CastTimer;
		GetWorldTimerManager().SetTimer(CastTimer, this, &AEtherealPlayerState::Regen, 1.0f, false);
	}	
}

// Refresh Tick
void AEtherealPlayerState::Refresh()
{
	if (!Player->IsDead)  // do not tick if Player is dead
	{
		MP_Current = MP_Current + RefreshRate;
		ForceMPCaps();
		// Call Refresh again after 1 second
		FTimerHandle CastTimer;
		GetWorldTimerManager().SetTimer(CastTimer, this, &AEtherealPlayerState::Refresh, 1.0f, false);
	}	
}

// Creates a Gear Item, when given a name
AEtherealGearMaster* AEtherealPlayerState::CreateGear(EMasterGearList ItemToCreate)
{
	AEtherealGearMaster* Item = nullptr; // set a false item

	switch (ItemToCreate)  // This switch will run through and spawn a piece of Gear based on the name
	{
	case EMasterGearList::GL_None:
		// Do nothing, because the item is set to NONE!
		break;
	case EMasterGearList::GL_Cure:
		Item = GetWorld()->SpawnActor<ACure>();
		break;
	case EMasterGearList::GL_Cure2:
		Item = GetWorld()->SpawnActor<ACureII>();
		break;
	case EMasterGearList::GL_Raise:
		// Do nothing, because Raise does not yet exist
		break;
	case EMasterGearList::GL_Regen:
		Item = GetWorld()->SpawnActor<ARegen>();
		break;
	case EMasterGearList::GL_Refresh:
		Item = GetWorld()->SpawnActor<ARefresh>();
		break;
	case EMasterGearList::GL_Esuna:
		// Do nothing, because Esuna does not yet exist
		break;
	case EMasterGearList::GL_Comet:
		// Do nothing, because Comet does not yet exist
		break;
	case EMasterGearList::GL_Blizzard:
		Item = GetWorld()->SpawnActor<ABlizzard>();
		break;
	case EMasterGearList::GL_Thunder:
		Item = GetWorld()->SpawnActor<AThunder>();
		break;
	case EMasterGearList::GL_Fire:
		Item = GetWorld()->SpawnActor<AFire>();
		break;
	case EMasterGearList::GL_Poison:
		// Do nothing, because Poison does not yet exist
		break;
	case EMasterGearList::GL_Sleep:
		// Do nothing, because Sleep does not yet exist
		break;
	case EMasterGearList::GL_Berserk:
		Item = GetWorld()->SpawnActor<ABerserk>();
		break;
	case EMasterGearList::GL_Barrier:
		Item = GetWorld()->SpawnActor<ABarrier>();
		break;
	case EMasterGearList::GL_Haste:
		Item = GetWorld()->SpawnActor<AHaste>();
		break;
	case EMasterGearList::GL_Slow:
		// Do nothing, because Slow does not yet exist
		break;
	case EMasterGearList::GL_Counter:
		// Do nothing, because Counter does not yet exist
		break;
	case EMasterGearList::GL_Stun:
		// Do nothing, because Stun does not yet exist
		break;
	case EMasterGearList::GL_Dash:
		Item = GetWorld()->SpawnActor<ADash>();
		break;
	case EMasterGearList::GL_Gravity:
		// Do nothing, because Gravity does not yet exist
		break;
	case EMasterGearList::GL_IronSword:
		Item = GetWorld()->SpawnActor<AIronSword>();
		break;
	case EMasterGearList::GL_Joyeuse:
		Item = GetWorld()->SpawnActor<AJoyeuse>();
		break;
	case EMasterGearList::GL_Hauteclaire:
		Item = GetWorld()->SpawnActor<AHauteclaire>();
		break;
	case EMasterGearList::GL_Excalibur:
		Item = GetWorld()->SpawnActor<AExcalibur>();
		break;
	case EMasterGearList::GL_WarAspis:
		Item = GetWorld()->SpawnActor<AWarAspis>();
		break;
	case EMasterGearList::GL_SpartanShield:
		Item = GetWorld()->SpawnActor<ASpartanShield>();
		break;
	case EMasterGearList::GL_ShortBow:
		Item = GetWorld()->SpawnActor<AShortBow>();
		break;
	case EMasterGearList::GL_EagleEye:
		Item = GetWorld()->SpawnActor<AEagleEye>();
		break;
	case EMasterGearList::GL_EurytosBow:
		Item = GetWorld()->SpawnActor<AEurytosBow>();
		break;
	case EMasterGearList::GL_Annihilator:
		Item = GetWorld()->SpawnActor<AAnnihilator>();
		break;
	case EMasterGearList::GL_DemonArrow:
		Item = GetWorld()->SpawnActor<ADemonArrow>();
		break;
	case EMasterGearList::GL_FlameArrow:
		// Do nothing, because Flame Arrow does not yet exist
		break;
	case EMasterGearList::GL_IronArrow:
		// Do nothing, because Iron Arrow does not yet exist
		break;
	case EMasterGearList::GL_WoodenArrow:
		Item = GetWorld()->SpawnActor<AWoodenArrow>();
		break;
	case EMasterGearList::GL_Juggernaut:
		Item = GetWorld()->SpawnActor<AJuggernaut>();
		break;
	case EMasterGearList::GL_Claymore:
		Item = GetWorld()->SpawnActor<AClaymore>();
		break;
	case EMasterGearList::GL_Marauder:
		Item = GetWorld()->SpawnActor<AMarauder>();
		break;
	case EMasterGearList::GL_Temperance:
		Item = GetWorld()->SpawnActor<ATemperance>();
		break;
	case EMasterGearList::GL_MagicOrbs:
		Item = GetWorld()->SpawnActor<AMagicOrbs>();
		break;
	case EMasterGearList::GL_Potion:
		Item = GetWorld()->SpawnActor<APotion>();
		break;
	case EMasterGearList::GL_HiPotion:
		Item = GetWorld()->SpawnActor<AHiPotion>();
		break;
	case EMasterGearList::GL_Ether:
		Item = GetWorld()->SpawnActor<AEther>();
		break;
	case EMasterGearList::GL_HiEther:
		Item = GetWorld()->SpawnActor<AHiEther>();
		break;
	case EMasterGearList::GL_Elixer:
		Item = GetWorld()->SpawnActor<AElixer>();
		break;
	case EMasterGearList::GL_Reraise:
		Item = GetWorld()->SpawnActor<AReraise>();
		break;
	case EMasterGearList::GL_Adrenaline:
		Item = GetWorld()->SpawnActor<AAdrenaline>();
		break;
	case EMasterGearList::GL_SentinelBrew:
		Item = GetWorld()->SpawnActor<ASentinelBrew>();
		break;
	}

	return Item;
}

// Attaches a piece of Gear to the player
void AEtherealPlayerState::AttachGear(AEtherealGearMaster* Item)
{
	if (Item) // be sure Item is valid
	{
		if (Item->OwnerReference) // ensure the item has a reference to the player
		{
			AWeapon_Master* Weapon = Cast<AWeapon_Master>(Item);  // check if the item is a weapon, and if so, store it's reference.

			switch (Item->Type)
			{
			case EMasterGearTypes::GT_None:
				break;
			case EMasterGearTypes::GT_OneHanded:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket_Right"));
				break;
			case EMasterGearTypes::GT_TwoHanded:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket_GreatSword"));
				break;
			case EMasterGearTypes::GT_Ranged:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket_Bow"));
				break;
			case EMasterGearTypes::GT_Casting:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("CastingOrb_R"));
				if (Weapon)
				{
					Weapon->WeaponOffhandMesh->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("CastingOrb_L"));
				}
				break;
			case EMasterGearTypes::GT_White:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			case EMasterGearTypes::GT_Black:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			case EMasterGearTypes::GT_Support:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			case EMasterGearTypes::GT_Ancient:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			case EMasterGearTypes::GT_Shield:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket_Shield"));
				break;
			case EMasterGearTypes::GT_Ammo:
				//Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket_Ammo"));
				break;
			case EMasterGearTypes::GT_Head:
				break;
			case EMasterGearTypes::GT_Cape:
				break;
			case EMasterGearTypes::GT_Body:
				break;
			case EMasterGearTypes::GT_Hands:
				break;
			case EMasterGearTypes::GT_Legs:
				break;
			case EMasterGearTypes::GT_Feet:
				break;
			case EMasterGearTypes::GT_Consumable:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			case EMasterGearTypes::GT_KeyItem:
				Item->AttachToComponent(Item->OwnerReference->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PowerSocket"));
				break;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, "Gear was attached to player.");
		}
	}	
}

// Get Inventory Item
AEtherealGearMaster* AEtherealPlayerState::GetInventoryItem(EMasterGearList ItemToGet)
{
	AEtherealGearMaster* Item = nullptr; // set a false item

	for (AEtherealGearMaster* GearItem : Inventory)  // scan the inventory...
	{
		if (GearItem->Name == ItemToGet)  // if ItemToGet matches the name of an item currently in the inventory
		{
			Item = GearItem;  // set the Item reference to be returned
			return Item;
		}
		else
		{
			Item = nullptr;  // no item name match was found, so return null
		}
	}
	return Item;
}

// Create and Add Inventory Item, based on given item name
void AEtherealPlayerState::AddToInventory(EMasterGearList ItemToAdd, bool ShouldBind)
{
	bool CanAdd = true;  // always start by assuming you can add the item

	

	if (Inventory.Num() > 0)  // check if the inventory is empty
	{
		for (AEtherealGearMaster* GearItem : Inventory)  // inventory wasn't empty, so for each item in the inventory...
		{
			if (GearItem->Name == ItemToAdd) // check if ItemToAdd matches anything currently in the inventory
			{
				CanAdd = false; // if so, set CanAdd to false
				IncrementItemQuantity(GearItem);  // Call this function to check if the item is consumable, and if so, increase it's quantity
			}
		}
	}

	if (CanAdd) // if CanAdd is still true, because no existing match was found in inventory
	{
		AEtherealGearMaster* Gear = CreateGear(ItemToAdd); // create the new item

		if (Gear) // ensure gear was created successfully
		{
			Gear->ConfigureGear(Player); // configure this gear item
			Inventory.AddUnique(Gear); // add the item to the player's inventory
			AttachGear(Gear); // attach the Gear to the player
			
			// Bind this Gear to the player if required
			if (ShouldBind)
			{
				if (Gear->Type == EMasterGearTypes::GT_Consumable || Gear->Type == EMasterGearTypes::GT_KeyItem)
				{
					return;  // do not bind if Gear is a Consumable or a Key Item
				}
				else
				{
					Gear->Bind();  // Bind the item if necessary (this is usually only true when first loading the game)
				}				
			}
		}		
	}
}

// Increment Item Quantity
void AEtherealPlayerState::IncrementItemQuantity(AEtherealGearMaster* Item)
{
	if (Item->Type == EMasterGearTypes::GT_Consumable) // check to ensure the item is consumable
	{
		AItem_Master* ConsumableItem = Cast<AItem_Master>(Item);  // cast to the Master Item class
		ConsumableItem->Quantity = ConsumableItem->Quantity + 1;  // increment the item's quantity by 1
	}
}

// ADD GEAR STATS
void AEtherealPlayerState::AddGearStats(EMasterGearList GearName)
{
	// Get the inventory item
	AEtherealGearMaster* Gear = GetInventoryItem(GearName);

	// Add stats from gear to the player's stats
	ATK = (ATK + Gear->ATK);
	DEF = (DEF + Gear->DEF);
	SPD = (SPD + Gear->SPD);
	HP_Max = (HP_Max + Gear->HP);
	MP_Max = (MP_Max + Gear->MP);
}

// REMOVE GEAR STATS
void AEtherealPlayerState::RemoveGearStats(EMasterGearList GearName)
{
	// Get the inventory item
	AEtherealGearMaster* Gear = GetInventoryItem(GearName);

	// Add stats from gear to the player's stats
	ATK = (ATK - Gear->ATK);
	DEF = (DEF - Gear->DEF);
	SPD = (SPD - Gear->SPD);
	HP_Max = (HP_Max - Gear->HP);
	MP_Max = (MP_Max - Gear->MP);
}

// Adds default items to the player's inventory. Used when starting a new game without previous save data, so the player doesn't have to start with nothing.
void AEtherealPlayerState::AddDefaultItems()
{
	AddToInventory(EMasterGearList::GL_IronSword, false);
	AddToInventory(EMasterGearList::GL_WarAspis, false);
	AddToInventory(EMasterGearList::GL_Cure, false);
	AddToInventory(EMasterGearList::GL_Thunder, false);
	AddToInventory(EMasterGearList::GL_Dash, false);
	AddToInventory(EMasterGearList::GL_Potion, false);
	AddToInventory(EMasterGearList::GL_Potion, false);
}

// Unbind all Items
void AEtherealPlayerState::UnbindAll()
{
	for (AEtherealGearMaster* GearItem : Inventory)  // for each item in the inventory...
	{
		GearItem->Unbind(); // Unbind item
		// TO DO : these should be destroyed after they're unbound, since this function is currently only being called from the main menu, when you've chosen to start a new game (which deletes your old save file)
	}

	// Set all Magic Bindings to None
	Binding_Magic_Slot1 = EMasterGearList::GL_None;
	Binding_Magic_Slot2 = EMasterGearList::GL_None;
	Binding_Magic_Slot3 = EMasterGearList::GL_None;
	Binding_Magic_Slot4 = EMasterGearList::GL_None;
	Binding_Magic_Slot5 = EMasterGearList::GL_None;
	Binding_Magic_Slot6 = EMasterGearList::GL_None;

	// Set All Weapon Bindings to None
	Binding_Weapon_Main = EMasterGearList::GL_None;
	Binding_Weapon_Sub = EMasterGearList::GL_None;

	Inventory.Empty();  // clear the player's inventory

	AddDefaultItems();  // Give the player default items, so they're not fully useless
}