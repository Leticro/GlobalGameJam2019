#include "Pickup.h"
#include "MoyoPlayerController.h"

APickup::APickup()
{
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    RootComponent = Cast<USceneComponent>(PickupMesh);

    ItemID = FName("No ID");

    Super::Name = "Item";
    Super::Action = "pickup";
}

void APickup::Interact_Implementation(APlayerController* Controller)
{
    Super::Interact_Implementation(Controller);

    AMoyoPlayerController* IController = Cast<AMoyoPlayerController>(Controller);
    if(IController->AddItemToInventoryByID(ItemID))
        Destroy();
}