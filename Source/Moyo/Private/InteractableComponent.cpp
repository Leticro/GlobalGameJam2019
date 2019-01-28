// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableComponent.h"

// Sets default values
UInteractableComponent::UInteractableComponent()
{
    Name = "Interactible";
    Action = "interact";
}

void UInteractableComponent::Interact_Implementation(APlayerController* Controller, AMoyoCharacter* Character)
{
    OnInteractEvent.Broadcast(Controller, Character);
    return;
}

FString UInteractableComponent::GetInteractText() const
{
    return FString::Printf(TEXT("%s: Press E to %s"), *Name, *Action);
}

