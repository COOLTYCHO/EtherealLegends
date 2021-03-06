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
#include "Armor_Master.h"

AArmor_Master::AArmor_Master(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Asset, Reference Obtained Via Right Click in Editor
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObject(TEXT("SkeletalMesh'/Game/VFX/sphere_skeletal.sphere_skeletal'"));

	// Set Default Objects
	SK_HeadArmorSkeletalMesh = SkeletalMeshObject.Object;

	// Create
	HeadArmorSkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Head Armor"));

	// Set Mesh
	HeadArmorSkeletalMesh->SetSkeletalMesh(SK_HeadArmorSkeletalMesh);

	// Configure Mesh
	UCommonLibrary::SetupSKComponentsWithCollision(HeadArmorSkeletalMesh);
}

// Called when the game starts or when spawned
void AArmor_Master::BeginPlay()
{
	Super::BeginPlay();

	// Attachment
	//HeadArmorSkeletalMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}


