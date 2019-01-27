#pragma once

#include "Engine.h"

#include "MoyoCharacter.h"
#include "Interactable.h"
#include "Pickup.generated.h"

UCLASS()
class MOYO_API APickup : public AInteractable
{
    GENERATED_BODY()

public:
    APickup();

    void Interact_Implementation(APlayerController* Controller, AMoyoCharacter* Character) override;

protected:
    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* PickupMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FName ItemID;
};