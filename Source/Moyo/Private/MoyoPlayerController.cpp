// Fill out your copyright notice in the Description page of Project Settings.

#include "Moyo/Public/MoyoPlayerController.h"

#include "MoyoGameState.h"
#include "InteractableComponent.h"

/* Started moving input logic into the Player Controller but task is on hold for now

void AMoyoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &AMoyoCharacter::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &AMoyoCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AMoyoCharacter::TouchStopped);
}


void AMoyoPlayerController::MoveRight(float Val)
{
	// add movement in that direction
	GetPawn()->AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

*/

AMoyoPlayerController::AMoyoPlayerController()
{
    InventorySlotLimit = 1;
    InventoryWeightLimit = 3;
}

int32 AMoyoPlayerController::GetInventoryWeight()
{
    int32 Weight = 0;
    for(auto& Item : Inventory)
    {
        Weight += Item.Weight;
    }

    return Weight;
}

bool AMoyoPlayerController::AddItemToInventoryByID(FName ID)
{
    AMoyoGameState* GameState = Cast<AMoyoGameState>(GetWorld()->GetGameState());
    UDataTable* ItemTable = GameState->GetItemDB();
    FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

    if(ItemToAdd)
    {
        // If a Slot- or WeightLimit are not needed remove them in this line
        if(Inventory.Num() < InventorySlotLimit && GetInventoryWeight() + ItemToAdd->Weight <= InventoryWeightLimit)
        {
            Inventory.Add(*ItemToAdd);
            ReloadInventory();
            return true;
        }
    }
    return false;
}

void AMoyoPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Interact", IE_Pressed, this, &AMoyoPlayerController::Interact);
}

void AMoyoPlayerController::Interact()
{
    // inventory full
    if(Inventory.Num())
    {
        // drop object
        Inventory.Pop();
        
    }
    else if(CurrentInteractable)
    {
        CurrentInteractable->Interact(this, CurrentPawn);
    }
    else if(CurrentInteractableComponent)
    {
        CurrentInteractableComponent->Interact(this, CurrentPawn);
    }
}
