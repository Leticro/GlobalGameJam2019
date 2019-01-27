// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MoyoCharacter.h"
#include "Interactable.h"
#include "InventoryItem.h"
#include "Pickup.h"
#include "MoyoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOYO_API AMoyoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    AMoyoPlayerController();

    UFUNCTION(BlueprintImplementableEvent)
        void ReloadInventory();

    UFUNCTION(BlueprintCallable, Category = "Utils")
        int32 GetInventoryWeight();

    UFUNCTION(BlueprintCallable, Category = "Utils")
        bool AddItemToInventoryByID(FName ID);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        class AInteractable* CurrentInteractable;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        class UInteractableComponent* CurrentInteractableComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        class AMoyoCharacter* CurrentPawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        TArray<FInventoryItem> Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 InventorySlotLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 InventoryWeightLimit;

    class APickup* CurrentObject;

protected:
    void Interact();

    virtual void SetupInputComponent() override;

	/*
	// Called for side to side input
	void MoveRight(float Val);
	*/
};
