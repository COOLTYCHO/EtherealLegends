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

#pragma once

#include "GameFramework/PlayerState.h"
#include "Gear/MasterGearList.h"
#include "EtherealPlayerState.generated.h"

class AEtherealPlayerMaster;
class AEtherealGearMaster;

UCLASS(Blueprintable)
class ETHEREAL_API AEtherealPlayerState : public APlayerState
{
	GENERATED_BODY()

	AEtherealPlayerState(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	
	///////////////////////////////////////////////////
	// STATS

	// A reference to the player pawn in game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	AEtherealPlayerMaster* Player;

	// The Player's current Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 PlayerLevel;

	// The player's current HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Current;

	// the player's Max HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Max;

	// The player's current MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MP_Current;

	// The player's Max MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MP_Max;

	// The player's current TP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float TP_Current;

	// The player's Max TP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float TP_Max;

	// The player's current ATK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 ATK;

	// the player's current DEF
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 DEF;

	// The Player's current SPD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 SPD;

	// The player's Refresh Rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float RefreshRate;

	// The player's Regen Rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float RegenRate;
	
	// Sets the initial stats of the player, based on his level
	UFUNCTION(BlueprintCallable, Category = Stats)
	void SetBaseStats();

	// Restores all HP and MP to full
	UFUNCTION(BlueprintCallable, Category = Stats)
	void RestoreAll();

	// Forces HP to be within 0 and HP Max
	UFUNCTION(BlueprintCallable, Category = Stats)
	void ForceHPCaps();

	// Forces MP to be within 0 and MP Max
	UFUNCTION(BlueprintCallable, Category = Stats)
	void ForceMPCaps();

	// Performs a single Regen tick
	UFUNCTION(BlueprintCallable, Category = Stats)
	void Regen();

	// Performs a single Refresh tick
	UFUNCTION(BlueprintCallable, Category = Stats)
	void Refresh();
	
	//////////////////////////////////////////////////////////
	// EXP

	// Current EXP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int32 EXP_Current;

	// EXP needed to gain the next level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int32 EXP_ToNextLevel;

	// EXP threshold of the previous level; used for displaying the EXP bar on the status screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int32 EXP_PreviousTNL;

	// Total number of enemies killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int32 KillCount;

	// Sets the "To Next Level" value after gaining a new level
	UFUNCTION(BlueprintCallable, Category = EXP)
	void SetTNL();

	// Rewards the player with EXP and Gold for killing an enemy
	UFUNCTION(BlueprintCallable, Category = EXP)
	void AddEXP(int modEXP);

	// Rewards the player with EXP and Gold for killing an enemy
	UFUNCTION(BlueprintCallable, Category = EXP)
	void AddGold(int modGold);

	// Rewards the player with EXP and Gold for killing an enemy. Called from the enemy, passing it's level.
	UFUNCTION(BlueprintCallable, Category = EXP)
	void DropItems(EMasterGearList Common, EMasterGearList Uncommon, EMasterGearList Rare);

	// Rewards the player with EXP and Gold for killing an enemy. Called from the enemy, passing it's level.
	UFUNCTION(BlueprintCallable, Category = EXP)
	void EnemyKillReward(int EnemyLevel, EMasterGearList Common, EMasterGearList Uncommon, EMasterGearList Rare);

	// Runs the Level Up functions
	UFUNCTION(BlueprintCallable, Category = EXP)
	void LevelUp();

	///////////////////////////////////////////////////////////
	// INVENTORY

	// Inventory Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<AEtherealGearMaster*> Inventory;

	// Current Gold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 Gold_Current;

	// Spawns a new Gear item, based on the Name
	UFUNCTION(BlueprintCallable, Category = Inventory)
	AEtherealGearMaster* CreateGear(EMasterGearList ItemToCreate);

	// Attaches the Gear item to the Player.
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AttachGear(AEtherealGearMaster* Item);

	// Function to get an item reference from the Inventory, based on the Name
	UFUNCTION(BlueprintCallable, Category = Inventory)
	AEtherealGearMaster* GetInventoryItem(EMasterGearList ItemToGet);

	// Function to add an item to the Inventory, based on the Name
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddToInventory(EMasterGearList ItemToAdd, bool ShouldBind);

	// Increment the Quantity of an Item, based on the Name. This function only gets called by AddToInventory(), in the event you already have a consumable item in your inventory when obtaining a new one.
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void IncrementItemQuantity(AEtherealGearMaster* Item);

	// Adds the stats of an equipped Gear Item to the player's base stats.
	UFUNCTION(BlueprintCallable, Category = Stats)
	void AddGearStats(EMasterGearList GearName);

	// Removes the stats of an equipped Gear Item from the player's base stats.
	UFUNCTION(BlueprintCallable, Category = Stats)
	void RemoveGearStats(EMasterGearList GearName);

	// Adds default items to the player's inventory. Used when starting a new game without previous save data.
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultItems();

	//////////////////////////////////////////////////////
	// BINDINGS

	// Unbinds all items
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void UnbindAll();

	// Magic Bindings

	// Tier 1 Slot 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot1;

	// Tier 1 Slot 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot2;

	// Tier 1 Slot 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot3;

	// Tier 2 Slot 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot4;

	// Tier 2 Slot 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot5;

	// Tier 2 Slot 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Magic_Slot6;

	// Weapon Bindings

	//  Main Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Weapon_Main;

	// Sub Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Weapon_Sub;

	//////////////////////////////////////////////////////////
	// ITEMS

	// Current Consumable Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Item_Current;

};
