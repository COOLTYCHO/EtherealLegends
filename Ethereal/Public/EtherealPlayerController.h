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

#include "GameFramework/PlayerController.h"
#include "Characters/CombatText/CombatEffectStruct.h"
#include "EtherealPlayerController.generated.h"

UCLASS()
class ETHEREAL_API AEtherealPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	AEtherealPlayerController();
		
	// All Possible Status Effects are stored here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatusEffects)
	TArray<FCombatEffectStruct> StatusEffects;

	/////////////////////////////////////////
	// Status Effect Blueprint Events
	// Because I have not yet translated them from BP to C++

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Heal();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Reraise();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Regen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Refresh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Barrier();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void RemoveStatus_Barrier();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Berserk();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Haste();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Poison();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = StatusEffects)
	void ActivateStatus_Burn();
};
