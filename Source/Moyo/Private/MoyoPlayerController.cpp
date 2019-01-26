// Fill out your copyright notice in the Description page of Project Settings.

#include "Moyo/Public/MoyoPlayerController.h"


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

