// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoyoCharacter.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractDelegate, APlayerController*, Controller, AMoyoCharacter*, Character);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOYO_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    UInteractableComponent();

    UFUNCTION(BlueprintNativeEvent)
        void Interact(APlayerController* Controller, AMoyoCharacter* Character);
    virtual void Interact_Implementation(APlayerController* Controller, AMoyoCharacter* Character);

    UPROPERTY(EditDefaultsOnly)
        FString Name;

    UPROPERTY(EditDefaultsOnly)
        FString Action;

    UPROPERTY(BlueprintAssignable, Category = "Interact")
    FInteractDelegate OnInteractEvent;

    UFUNCTION(BlueprintCallable, Category = "Pickup")
        FString GetInteractText() const;
};
