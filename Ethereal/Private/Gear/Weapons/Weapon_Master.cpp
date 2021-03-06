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
#include "Weapon_Master.h"

AWeapon_Master::AWeapon_Master(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Asset, Reference Obtained Via Right Click in Editor
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObject(TEXT("SkeletalMesh'/Game/VFX/sphere_skeletal.sphere_skeletal'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObject(TEXT("StaticMesh'/Game/VFX/sphere.sphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OffhandMeshObject(TEXT("StaticMesh'/Game/VFX/sphere.sphere'"));

	// Set Default Objects
	SK_WeaponSkeletalMesh = SkeletalMeshObject.Object;
	SM_WeaponStaticMesh = StaticMeshObject.Object;
	SM_WeaponOffhandMesh = StaticMeshObject.Object;

	// Create
	WeaponSkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Weapon Skeletal Mesh"));
	WeaponSkeletalMesh->SetSkeletalMesh(SK_WeaponSkeletalMesh);
	WeaponSkeletalMesh->SetupAttachment(Root);
	WeaponSkeletalMesh->SetVisibility(false);

	WeaponStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Weapon Static Mesh"));
	WeaponStaticMesh->SetStaticMesh(SM_WeaponStaticMesh);
	WeaponStaticMesh->SetupAttachment(Root);
	WeaponStaticMesh->SetVisibility(false);

	WeaponOffhandMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Weapon Offhand Mesh"));
	WeaponOffhandMesh->SetStaticMesh(SM_WeaponOffhandMesh);
	WeaponOffhandMesh->SetupAttachment(Root);
	WeaponOffhandMesh->SetVisibility(false);

	// Configure Mesh
	UCommonLibrary::SetupSKComponentsWithCollision(WeaponSkeletalMesh);
	UCommonLibrary::SetupSMComponentsWithCollision(WeaponStaticMesh);
	UCommonLibrary::SetupSMComponentsWithCollision(WeaponOffhandMesh);
}

// Called when the game starts or when spawned
void AWeapon_Master::BeginPlay()
{
	Super::BeginPlay();

	OnRemoveGear.AddDynamic(this, &AWeapon_Master::RemoveWeapon);
}

void AWeapon_Master::OnHit(UObject* CharacterMaster)
{

}

// Custom code when removing binding
void AWeapon_Master::RemoveWeapon()
{
	WeaponSkeletalMesh->SetVisibility(false);
	WeaponStaticMesh->SetVisibility(false);
	WeaponOffhandMesh->SetVisibility(false);
}



