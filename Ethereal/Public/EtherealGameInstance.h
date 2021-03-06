// � 2014 - 2016 Soverance Studios
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

#include "Engine/GameInstance.h"
#include "Management/GameStates.h"
#include "Management/Realms.h"
#include "EtherealGameInstance.generated.h"

UCLASS()
class ETHEREAL_API UEtherealGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// The current state of the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EGameStates CurrentState;

	// The current gameplay Realm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	ERealms CurrentRealm;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Action)
	void LoadNewRealm(ERealms RealmToLoad);
};
