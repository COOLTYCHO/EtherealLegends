// © 2014 - 2016 Soverance Studios. All Rights Reserved.

#pragma once

#include "Gear/EtherealGearMaster.h"
#include "Item_Master.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_API AItem_Master : public AEtherealGearMaster
{
	GENERATED_BODY()
	
public:

	AItem_Master(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/** The current quantity of this item stored in the player's inventory. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int Quantity;

	/** What effect this item has on gameplay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Details)
	FString Effect;
	
	/** This emitter is active when this item is used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystem* ItemFX;

	// Item Use Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UAudioComponent* ItemAudio;
	USoundCue* S_ItemAudio;
	
};
