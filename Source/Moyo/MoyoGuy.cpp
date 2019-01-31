// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoGuy.h"
#include "Moyo.h"
#include "MoyoMotor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

#include "Moyo/Public/MoyoCharacterMovementComponent.h"
#include "Moyo/Public/MoyoPlayerController.h"
#include "DrawDebugHelpers.h"

AMoyoGuy::AMoyoGuy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMoyoCharacterMovementComponent>(CharacterMovementComponentName))
{
	
	MoyoCharMovementComp = Cast<UMoyoCharacterMovementComponent>(GetMovementComponent());

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 70.0f);

	// rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	motor = CreateDefaultSubobject<UMoyoMotor>(TEXT("motor"));


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1100.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;


	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


void AMoyoGuy::BeginPlay()
{
	Super::BeginPlay();

    speed = 24.0f;
	
	inputDir = 1.0f;

}

// Called every frame
void AMoyoGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetActorLocation().Z < -500)
	{
		
		DoFallOut();
	}

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		footRemaining -= DeltaTime;
		if (footRemaining < 0)
		{
			footRemaining = 1.0f;
			DoKeepWalking();
		}
	}

}



void AMoyoGuy::MoveRight(float Value)
{
	if (Value != 0)
		inputDir = Value;
    
	switch (motor->motorState)
	{
	case EMoyoMotorState::CYLINDER:
		MoveRightCylinder(Value);
		break;
	case EMoyoMotorState::LINEAR:
		MoveRightLinear(Value);
		break;
	default:
		break;
	}
}


void AMoyoGuy::MoveRightCylinder(float Value)
{
	float factor = 0.1f* FMath::Abs(FMath::Sin(FMath::DegreesToRadians(Value / 2.0f)));
	if (Value > 0)
	{
		factor *= -1;
	}
	FVector next = UMoyoLib::GetMoveDestination(1.0f, this,factor, motor);
	FVector delta = next - GetActorLocation();
	FVector dir;
	float scalar;
	delta.ToDirectionAndLength(dir, scalar);
	AddMovementInput(dir, scalar);
}

void AMoyoGuy::MoveRightLinear(float Value)
{
	// add movement in the direction
	AddMovementInput(-motor->lineDirection, speed * Value);

	FVector next = UMoyoLib::GetMoveDestination(1.0f, this, speed*Value, motor);
	FVector delta = next - GetActorLocation();
	FVector dir;
	float scalar;
	delta.ToDirectionAndLength(dir, scalar);
	AddMovementInput(dir, scalar);

}


void AMoyoGuy::DoDeath_Implementation()
{
	FVector LaunchVector = 99.9f * GetActorLocation();
	LaunchCharacter(LaunchVector, false, false);
	motor->motorState = EMoyoMotorState::NONE;
}

void AMoyoGuy::DoFallOut_Implementation()
{
	SetActorLocation(motor->lastGoodPosition);
}


void AMoyoGuy::SendItem(FName itemID)
{
	if (RemainingItems.Contains(itemID)) 
	{ 
		DoRecieve(itemID); 
	} 
}
