// © 2014 - 2016 Soverance Studios. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Gear/EtherealGearMaster.h"
#include "EtherealPlayerState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ETHEREAL_API AEtherealPlayerState : public APlayerState
{
	GENERATED_BODY()

	AEtherealPlayerState(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	
	// STATS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MP_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MP_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float TP_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float TP_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int DEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int SPD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float RefreshRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float RegenRate;
	

	UFUNCTION(BlueprintCallable, Category = Stats)
	void SetBaseStats();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void RestoreAll();

	UFUNCTION(BlueprintCallable, Category = Stats)
	bool ForceHPCaps();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void ForceMPCaps();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void Regen();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void Refresh();
	
	// EXP

	// Current EXP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int EXP_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int EXP_ToNextLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EXP)
	int EXP_PreviousTNL;

	UFUNCTION(BlueprintCallable, Category = EXP)
	void SetTNL();

	UFUNCTION(BlueprintCallable, Category = EXP)
	void LevelUp();

	// INVENTORY

	// Inventory Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<AEtherealGearMaster*> Inventory;

	// Current Gold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int Gold_Current;

	// Function to get an item reference from the Inventory, based on the Name
	UFUNCTION(BlueprintCallable, Category = Inventory)
	AEtherealGearMaster* GetInventoryItem(EMasterGearList ItemToGet);

	// Function to add an item to the Inventory, based on the Name
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddInventoryItem(EMasterGearList ItemToAdd);

	// Adds the stats of an equipped Gear Item to the player's base stats.
	UFUNCTION(BlueprintCallable, Category = Stats)
	void AddGearStats(EMasterGearList GearName);

	// Removes the stats of an equipped Gear Item from the player's base stats.
	UFUNCTION(BlueprintCallable, Category = Stats)
	void RemoveGearStats(EMasterGearList GearName);

	// BINDINGS

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Weapon_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Weapon_Sub;

	// Current Consumable Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Binding_Item_Current;

};
