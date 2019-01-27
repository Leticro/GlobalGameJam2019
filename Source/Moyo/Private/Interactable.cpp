// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
    Name = "Interactible";
    Action = "interact";
}

void AInteractable::Interact_Implementation(APlayerController* Controller)
{
    return;
}

FString AInteractable::GetInteractText() const
{
    return FString::Printf(TEXT("%s: Press E to %s"), *Name, *Action);
}

