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

#include "NPCs/EtherealNPCMaster.h"
#include "Gatekeeper.generated.h"

UCLASS()
class ETHEREAL_API AGatekeeper : public AEtherealNPCMaster
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AGatekeeper(const FObjectInitializer& ObjectInitializer);

	// BeginPlay Override
	virtual void BeginPlay() override;

	// Mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	USkeletalMeshComponent* Mesh;
	USkeletalMesh* SM_Mesh;

	// Interact with this NPC
	UFUNCTION(BlueprintCallable, Category = Default)
	void Interact();

	// Interact Widget.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSubclassOf<UUserWidget> W_InteractWidget;
	UUserWidget* InteractWidget;

	// Hide the Interact Widget for this NPC
	UFUNCTION(BlueprintCallable, Category = Default)
	void DisableInteractWidget();

	// Explain Anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool DoExplain;

	// Huh Anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool DoHuh;

	// Warning Anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool DoWarning;
};
