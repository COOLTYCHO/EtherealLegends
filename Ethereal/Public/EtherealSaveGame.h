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

#include "GameFramework/SaveGame.h"
#include "MasterGearList.h"
#include "EtherealSaveGame.generated.h"

UCLASS()
class ETHEREAL_API UEtherealSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UEtherealSaveGame();

	// Save Slot Name
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	///////////////////////////////////////
	// BINDINGS

	// MAGIC Tier 1 Slot 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot1;

	// MAGIC Tier 1 Slot 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot2;

	// MAGIC Tier 1 Slot 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot3;

	// MAGIC Tier 2 Slot 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot4;

	// MAGIC Tier 2 Slot 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot5;

	// MAGIC Tier 2 Slot 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Magic_Slot6;

	// Weapon Main
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Weapon_Main;

	// Weapon Sub
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Weapon_Sub;

	/////////////////////////////////////////
	//  STATS
	
	// Player Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 Save_PlayerLevel;

	// Current Gold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 Save_CurrentGold;

	// Current EXP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 Save_CurrentEXP;

	// Kill Count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 Save_KillCount;

	// Inventory Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<EMasterGearList> Save_Inventory;

	// Current Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bindings)
	EMasterGearList Save_Item_Current;
	
	/////////////////////////////////////////////
	// CONSUMABLE ITEM QUANTITIES

	// Potion Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyPotion;

	// Hi Potion Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyHiPotion;

	// Ether Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyEther;

	// Hi Ether Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyHiEther;

	// Elixer Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyElixer;

	// Reraise Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyReraise;

	// Adrenaline Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtyAdrenaline;

	// SentinelBrew Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quantities)
	int32 Save_QtySentinelBrew;
};
