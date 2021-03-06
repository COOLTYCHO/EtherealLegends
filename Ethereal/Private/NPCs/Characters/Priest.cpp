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
#include "Blueprint/UserWidget.h"
#include "Priest.h"

APriest::APriest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("SkeletalMesh'/Game/EtherealParty/OldWizard/OldWizard.OldWizard'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/EtherealParty/OldWizard/Anim_OldWizard.Anim_OldWizard'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("/Game/Blueprints/Widgets/BetaUnavailable"));

	// Set Default Objects
	SM_Mesh = NPCMesh.Object;
	W_InteractWidget = Widget.Class;

	// Create objects
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	Mesh->SkeletalMesh = SM_Mesh;
	Mesh->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	IsUsable = true;
	InteractAnimType = EInteractAnims::IA_Talk;

	DoInteract.AddDynamic(this, &APriest::Interact);
}

// Called when the game starts or when spawned
void APriest::BeginPlay()
{
	Super::BeginPlay();
}

// Interact with this NPC
void APriest::Interact()
{
	IsUsable = false;

	InteractWidget = CreateWidget<UUserWidget>(GetWorld(), W_InteractWidget);  // creates the widget
	InteractWidget->AddToViewport();

	// TO DO : HIDE THE BATTLE HUD

	// Hide the widget after a short delay
	FTimerHandle DisappearTimer;
	GetWorldTimerManager().SetTimer(DisappearTimer, this, &APriest::DisableInteractWidget, 3.0f, false);
}

// Disabled the Interact Widget associated with this NPC
void APriest::DisableInteractWidget()
{
	InteractWidget->RemoveFromViewport();
	IsUsable = true;
}