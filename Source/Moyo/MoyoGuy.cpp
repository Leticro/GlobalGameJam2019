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
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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
	GetCharacterMovement()->JumpZVelocity = 1000.f;
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

	defaultGravityScale = MoyoCharMovementComp->GravityScale;
    
    speed = 24.0f;
	
	inputDir = 1.0f;

}

// Called every frame
void AMoyoGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Correct location if outside rails
    FVector location = GetActorLocation();
    FVector elevation = FVector(0.0f, 0.0f, location.Z);
    location.Z = 0.0f;

    if(motor->isCylinder) { // --- Cylinder

        // Character Position
        FVector currentRadius = location - motor->cylinderFocus;
        currentRadius.Normalize();
        SetActorLocation(motor->cylinderFocus + elevation + currentRadius * motor->cylinderRadius);
        
        
    }else{ // --- Line
        // This should not happen often, so it's ok if we value speed over accuracy

        FVector sToP = location - motor->lineStartPoint;
        FVector newLocation = motor->lineDirection * sToP.Size();
        SetActorLocation(motor->lineStartPoint + elevation + newLocation);
        
        FVector cameraDir = FVector::CrossProduct(motor->lineDirection, FVector(0.0f, 0.0f, 1.0f));
        
    }

}



void AMoyoGuy::MoveRight(float Value)
{
	if (Value != 0)
		inputDir = Value;
    
    if(motor->isCylinder) { // --- Cylinder
		MoveRightCylinder(Value);
    
	}else{ // --- Line
		MoveRightLinear(Value);
        
    }
}


void AMoyoGuy::MoveRightCylinder(float Value)
{
	// Find new movement direction
	FVector location = GetActorLocation();

	float angle = speed * Value;

	FVector radius = location - motor->cylinderFocus;
	FVector newRadius = radius.RotateAngleAxis(angle, FVector(0.0f, 0.0f, 1.0f));

	FVector tangent = newRadius - radius;
	tangent.Normalize();

	float distance = 10.0f * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(angle / 2.0f)));

	// add movement in that direction
	
	//UE_LOG(LogTemp, Warning, TEXT("location: %s, dist: %f, tangent: %s"), *location.ToString(), distance, *(-tangent).ToString());
	AddMovementInput(-tangent, distance);
}

void AMoyoGuy::MoveRightLinear(float Value)
{
	// add movement in the direction
	AddMovementInput(-motor->lineDirection, speed * Value);
}

