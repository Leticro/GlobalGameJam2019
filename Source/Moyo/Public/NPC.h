#pragma once

#include "Engine.h"
#include "Interactable.h"
#include "NPC.generated.h"

UCLASS()
class MOYO_API ANPC : public AInteractable
{
    GENERATED_BODY()

public:
    ANPC();

    void Interact_Implementation(APlayerController* Controller, AMoyoCharacter* Character) override;

protected:
    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* NPCMesh;

};