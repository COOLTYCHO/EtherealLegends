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
#include "Chest.h"

AChest::AChest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("StaticMesh'/Game/ExampleContent/NetworkFeatures/Meshes/SM_Chest_Bottom.SM_Chest_Bottom'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LidMesh(TEXT("StaticMesh'/Game/ExampleContent/NetworkFeatures/Meshes/SM_Chest_Lid.SM_Chest_Lid'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> OpenAudioObject(TEXT("SoundCue'/Game/Audio/Cave/Ethereal_ChestOpen_Cue.Ethereal_ChestOpen_Cue'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> OpenParticle(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	
	// Set Default Objects
	SM_Base = BaseMesh.Object;
	SM_Lid = LidMesh.Object;
	S_OpenAudio = OpenAudioObject.Object;
	P_OpenFX = OpenParticle.Object;

	Root->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	InteractBox->SetBoxExtent(FVector(120, 120, 100));
	
	// Create objects
	Base = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Base"));
	Base->SetupAttachment(Root);
	Base->StaticMesh = SM_Base;

	Lid = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Lid"));
	Lid->SetupAttachment(Base);
	Lid->StaticMesh = SM_Lid;
	Lid->SetRelativeLocation(FVector(34, 0, 50));
	Lid->SetRelativeRotation(FRotator(0, -180, 0));

	OpenAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("OpenAudio"));
	OpenAudio->SetupAttachment(Root);
	OpenAudio->Sound = S_OpenAudio;
	OpenAudio->bAutoActivate = false;

	OpenFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("OpenFX"));
	OpenFX->SetupAttachment(Root);
	OpenFX->Template = P_OpenFX;
	OpenFX->bAutoActivate = false;
	OpenFX->SetRelativeLocation(FVector(-36, 0, 3));
	OpenFX->SetRelativeScale3D(FVector(1, 0.6f, 1));

	IsUsable = true;
	InteractAnimType = EInteractAnims::IA_Kick;
	ItemInChest = EMasterGearList::GL_None;  // set variable on instance in map
	GoldInChest = 0;  // set variable on instance in map

	DoInteract.AddDynamic(this, &AChest::Interact);
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
}

// Interact with this NPC
void AChest::Interact()
{
	if (IsUsable)
	{
		IsUsable = false;
		// Open chest during the kick animation
		FTimerHandle ChestTimer;
		GetWorldTimerManager().SetTimer(ChestTimer, this, &AChest::OnChestTouched, 0.5f, false);
	}
}

// Called when this chest is touched by the player
void AChest::OnChestTouched()
{
	OpenAudio->Play();
	// TO DO : Rotate the lid 180 degrees, so it appears to pop open	
	OpenFX->Activate();
	// TO DO : Play World Camera Shake Blueprint'/Game/Blueprints/CamShakes/CS_OpenChest.CS_OpenChest'
	// TO DO : delay 0.5
	// TO DO :  Spawn obtained item widget and add to viewport WidgetBlueprint'/Game/Blueprints/Widgets/ObtainedItemDisplay.ObtainedItemDisplay'
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "Opened Chest.");

	if (ItemInChest == EMasterGearList::GL_None)
	{
		AddGold();
	}
	else
	{
		AddItem();
	}

	// TO DO :  delay 2 seconds
	// fade out item display widget
	// delay 4 seconds
	// remove widget from screen
}

// Player obtains Gold
void AChest::AddGold()
{
	GoldInChest = FMath::RandRange(500, 5000);
	InteractingPlayer->EtherealPlayerState->Gold_Current = InteractingPlayer->EtherealPlayerState->Gold_Current + GoldInChest;
	// TO DO : update item display with gold obtained
}

// Player obtains Item
void AChest::AddItem()
{
	InteractingPlayer->EtherealPlayerState->AddToInventory(ItemInChest, false);
	// TO DO :  check the player's inventory. 
	// If the item in the chest is consumable, add it or increase quantity
	// if the item is already in the inventory and is not consumable, give the player Gold instead
	// if the item is not already in the inventory, add it
	// whatever the outcome, update the item display widget with the results

}
