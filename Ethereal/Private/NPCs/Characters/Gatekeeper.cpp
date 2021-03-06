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
#include "Gatekeeper.h"

AGatekeeper::AGatekeeper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram.SK_CharM_Ram'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_Mannequin_Skeleton_AnimBlueprint.SK_Mannequin_Skeleton_AnimBlueprint'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("/Game/Blueprints/Widgets/Tutorial"));

	// Set Default Objects
	SM_Mesh = NPCMesh.Object;
	W_InteractWidget = Widget.Class;
	
	// Create objects
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetAnimInstanceClass(AnimBP.Object->GetAnimBlueprintGeneratedClass());
	Mesh->SkeletalMesh = SM_Mesh;
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	IsUsable = true;
	InteractAnimType = EInteractAnims::IA_Talk;
	DoInteract.AddDynamic(this, &AGatekeeper::Interact);

	DoExplain = false;
	DoHuh = false;
	DoWarning = false;
}

// Called when the game starts or when spawned
void AGatekeeper::BeginPlay()
{
	Super::BeginPlay();
}

// Interact with this NPC
void AGatekeeper::Interact()
{
	IsUsable = false;

	InteractWidget = CreateWidget<UUserWidget>(GetWorld(), W_InteractWidget);  // creates the widget
	InteractWidget->AddToViewport();

	DoHuh = true;

	// Hide the widget after a short delay
	FTimerHandle DisappearTimer;
	GetWorldTimerManager().SetTimer(DisappearTimer, this, &AGatekeeper::DisableInteractWidget, 3.0f, false);
}

// Disabled the Interact Widget associated with this NPC
void AGatekeeper::DisableInteractWidget()
{
	InteractWidget->RemoveFromViewport();
	IsUsable = true;
}