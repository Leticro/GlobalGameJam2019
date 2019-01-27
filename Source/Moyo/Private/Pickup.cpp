#include "Pickup.h"
#include "Components/StaticMeshComponent.h"
#include "MoyoPlayerController.h"

APickup::APickup()
{
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    RootComponent = Cast<USceneComponent>(PickupMesh);

    ItemID = FName("No ID");

    Super::Name = "Item";
    Super::Action = "Pickup";
}

void APickup::Interact_Implementation(APlayerController* Controller)
{
    Super::Interact_Implementation(Controller);

    AMoyoPlayerController* IController = Cast<AMoyoPlayerController>(Controller);

    if(IController->AddItemToInventoryByID(ItemID))
    {
        UE_LOG(LogTemp, Warning, TEXT("PICKUP"));
        // Play animation and attach to hand
    }
}