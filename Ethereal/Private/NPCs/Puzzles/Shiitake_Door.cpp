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
#include "Characters/Player/EtherealPlayerMaster.h"
#include "Shiitake_Door.h"

AShiitake_Door::AShiitake_Door(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GearFrontMesh(TEXT("StaticMesh'/Game/InfinityBladeFireLands/Environments/Fire/Env_Fire_FlameDungeon/StaticMesh/Background/SM_Background_Gear01.SM_Background_Gear01'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> GateAudioObject(TEXT("SoundCue'/Game/Audio/Party/Shiitake_Gate_Cue.Shiitake_Gate_Cue'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColumnLeftMesh(TEXT("StaticMesh'/Game/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Pillar_2.SM_Ice_Fort_Pillar_2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColumnRightMesh(TEXT("StaticMesh'/Game/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Pillar_2.SM_Ice_Fort_Pillar_2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMesh(TEXT("StaticMesh'/Game/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Gate_1.SM_Ice_Fort_Gate_1'"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Blueprints/NPCs/Puzzles/Curve_ShiitakeGate.Curve_ShiitakeGate'"));

	// Set Default Objects
	SM_Gate1_Gear = GearFrontMesh.Object;
	S_GateAudio = GateAudioObject.Object;
	SM_Gate1_ColumnLeft = ColumnLeftMesh.Object;
	SM_Gate1_ColumnRight = ColumnRightMesh.Object;
	SM_Gate1 = GateMesh.Object;
	GateCurve = Curve.Object;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));

	InteractBox->SetRelativeLocation(FVector(0, 0, -200)); // move the regular InteractBox away from the area, since we built custom boxes for this NPC

	InteractBox_Gate1 = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("InteractBox_Gate1"));
	InteractBox_Gate1->SetupAttachment(Root);
	InteractBox_Gate1->SetBoxExtent(FVector(100, 100, 100));
	InteractBox_Gate1->SetRelativeLocation(FVector(288, 35, -170));
	InteractBox_Gate1->SetRelativeRotation(FRotator(0, 30, 0));

	Gate1_Gear = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Gate1_Gear"));
	Gate1_Gear->StaticMesh = SM_Gate1_Gear;
	Gate1_Gear->SetupAttachment(Root);
	UCommonLibrary::SetupSMComponentsWithCollision(Gate1_Gear);
	Gate1_Gear->SetRelativeLocation(FVector(275, -5, -75));
	Gate1_Gear->SetRelativeRotation(FRotator(5, 0, 90));
	Gate1_Gear->SetRelativeScale3D(FVector(1, 1, 3));

	GateAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("GateAudio"));
	GateAudio->SetupAttachment(Root);
	GateAudio->Sound = S_GateAudio;
	GateAudio->bAutoActivate = false;

	Gate1_ColumnLeft = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ColumnLeft"));
	Gate1_ColumnLeft->StaticMesh = SM_Gate1_ColumnLeft;
	Gate1_ColumnLeft->SetupAttachment(GateAudio);
	Gate1_ColumnLeft->SetRelativeLocation(FVector(-175, 0, 0));
	Gate1_ColumnLeft->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.4f));

	Gate1_ColumnRight = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ColumnRight"));
	Gate1_ColumnRight->StaticMesh = SM_Gate1_ColumnRight;
	Gate1_ColumnRight->SetupAttachment(GateAudio);
	Gate1_ColumnRight->SetRelativeLocation(FVector(175, 0, 0));
	Gate1_ColumnRight->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.4f));

	Gate1 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Gate"));
	Gate1->StaticMesh = SM_Gate1;
	Gate1->SetupAttachment(GateAudio);
	Gate1->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.4f));

	GateTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("GateTimeline"));
	InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });

	IsUsable = true;
	InteractAnimType = EInteractAnims::IA_Open;
}

// Called when the game starts or when spawned
void AShiitake_Door::BeginPlay()
{
	Super::BeginPlay();

	RotatingMovement->RotationRate = FRotator(0, 0, 0);

	GateStartLocation = Gate1->GetComponentLocation();


	InteractBox_Gate1->OnComponentBeginOverlap.AddDynamic(this, &AEtherealNPCMaster::OnInteract);
	InteractBox_Gate1->OnComponentEndOverlap.AddDynamic(this, &AEtherealNPCMaster::EndInteract);


	DoInteract.AddDynamic(this, &AShiitake_Door::Interact);
}

// Interact with this NPC
void AShiitake_Door::Interact()
{
	GateAudio->Play();
	RotatingMovement->SetUpdatedComponent(Gate1_Gear);
	RotatingMovement->RotationRate = FRotator(0, 90, 0);

	GateTimeline->AddInterpFloat(GateCurve, InterpFunction, FName{ TEXT("Float") });
	GateTimeline->PlayFromStart();

	IsUsable = false;

	// Do the stomp blast after the timeline completes
	FTimerHandle GearTimer;
	GetWorldTimerManager().SetTimer(GearTimer, this, &AShiitake_Door::StopGears, 6.0f, false);
}

// Gate Timeline
void AShiitake_Door::TimelineFloatReturn(float val)
{
	Gate1->SetRelativeLocation(FVector(GateStartLocation.X, GateStartLocation.Y, FMath::Lerp(0, -450, val))); // lerps from the door's starting Z location to Z location plus 90
}

// Stop Gear Rotation
void AShiitake_Door::StopGears()
{
	RotatingMovement->RotationRate = FRotator(0, 0, 0);
}