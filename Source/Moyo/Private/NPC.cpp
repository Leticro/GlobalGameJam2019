#include "NPC.h"
#include "Components/StaticMeshComponent.h"
#include "MoyoPlayerController.h"

ANPC::ANPC()
{
    NPCMesh = CreateDefaultSubobject<UStaticMeshComponent>("NPCMesh");
    RootComponent = Cast<USceneComponent>(NPCMesh);

    Super::Name = "Neighbor";
    Super::Action = "Talk";
}

void ANPC::Interact_Implementation(APlayerController* Controller)
{
    Super::Interact_Implementation(Controller);
}