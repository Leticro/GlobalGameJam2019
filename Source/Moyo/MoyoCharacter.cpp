// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Moyo/Public/MoyoCharacterMovementComponent.h"
#include "Moyo/Public/MoyoPlayerController.h"
#include "DrawDebugHelpers.h"

AMoyoCharacter::AMoyoCharacter(const FObjectInitializer& ObjectInitializer) 
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UMoyoCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMoyoCharacterMovementComponent>(CharacterMovementComponentName))
{
	
	MoyoCharMovementComp = Cast<UMoyoCharacterMovementComponent>(GetMovementComponent());

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

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


void AMoyoCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoyoPlayerController = (AMoyoPlayerController*) GetWorld()->GetFirstPlayerController();
	
	// Temporary measure to be able to see cursor
	//FInputModeGameAndUI defaultInputMode;
	//defaultInputMode.SetHideCursorDuringCapture(false);
	//MoyoPlayerController->SetInputMode(defaultInputMode);

	bSlingHeld = false;
    
    speed = 24.0f;
    cameraDistance = 75.0f;
    
    // TEMP
    SetCylinder(FVector(1200.0f, 20.0f, 0.0f), 500.0f);
    //SetLine(FVector(1200.0f, 20.0f, 0.0f), FVector(1800.0f, 50.0f, 0.0f));
}

// Called every frame
void AMoyoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    // Correct location if outside rails
    FVector location = GetActorLocation();
    FVector elevation = FVector(0.0f, 0.0f, location.Z);
    location.Z = 0.0f;
    
    if(isCylinder) { // --- Cylinder
        
        // Character Position
        FVector currentRadius = location - cylinderFocus;
        currentRadius.Normalize();
        SetActorLocation(cylinderFocus + elevation + currentRadius * cylinderRadius);
        
        // Camera Position/Rotation
        CameraBoom->SetWorldRotation((-currentRadius).Rotation());
        
    }else{ // --- Line
        // This should not happen often, so it's ok if we value speed over accuracy
        
        FVector sToP = location - lineStartPoint;
        FVector newLocation = lineDirection * sToP.Size();
        SetActorLocation(lineStartPoint + elevation + newLocation);
        
        FVector cameraDir = FVector::CrossProduct(lineDirection, FVector(0.0f, 0.0f, 1.0f));
        
        CameraBoom->SetWorldRotation((-cameraDir).Rotation());
    }

	//UE_LOG(LogTemp, Warning, TEXT("MoyoCharacter: Tick was called"));
	if (bSlingHeld)
	{
		SlingUpdateTrajectory(DeltaTime);
	}

}


void AMoyoCharacter::SlingUpdateTrajectory(float DeltaTime)
{
	FVector planeNormal = -GetControlRotation().RotateVector(FVector(1.0f, 0.f, 0.f));

	FVector actorPos = GetActorLocation();

	FVector mousePos;
	FVector mouseDir;
	MoyoPlayerController->DeprojectMousePositionToWorld(mousePos, mouseDir);

	FVector cursorLocation;
	if (LinePlaneIntersection(actorPos, planeNormal, mousePos, mouseDir, cursorLocation))
	{
		FVector slingVec = cursorLocation - actorPos;
		slingMag = slingVec.Size();
		slingVec.Normalize();
		slingDir = slingVec;
	
		UWorld* world = GetWorld();

		//DrawDebugLine(world, actorPos, )
		DrawDebugCircle(world, actorPos, minSlingRadius, 32, FColor::Yellow);
		DrawDebugCircle(world, actorPos, maxSlingRadius, 32, FColor::Yellow);
		DrawDebugSphere(world, cursorLocation, 5.0f, 12, FColor::Yellow);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMoyoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Sling", IE_Pressed, this, &AMoyoCharacter::SlingDown);
    PlayerInputComponent->BindAction("Sling", IE_Released, this, &AMoyoCharacter::SlingUp);
    
    PlayerInputComponent->BindAxis("MoveRight", this, &AMoyoCharacter::MoveRight);
}

void AMoyoCharacter::SetCylinder(FVector center, float radius) {
    isCylinder = true;
    
    cylinderFocus = center;
    cylinderRadius = radius;
}

void AMoyoCharacter::SetLine(FVector start, FVector end) {
    isCylinder = false;
    
    lineStartPoint = start;
    lineEndPoint = end;
    lineDirection = lineEndPoint - lineStartPoint;
    lineDirection.Z = 0.0f;
    lineDirection.Normalize();
}

void AMoyoCharacter::MoveRight(float Value)
{
    // Find new movement direction
    FVector location = GetActorLocation();
    
    if(isCylinder) { // --- Cylinder
        float angle = speed * Value;
        
        FVector radius = location - cylinderFocus;
        FVector newRadius = radius.RotateAngleAxis(angle, FVector(0.0f, 0.0f, 1.0f));
        
        FVector tangent = newRadius - radius;
        tangent.Normalize();
        
        float distance = 10.0f * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(angle/2.0f)));
        
        // add movement in that direction
        AddMovementInput(-tangent, distance);
    }else{ // --- Line
        
        // add movement in the direction
        AddMovementInput(-lineDirection, speed * Value);
        
    }
}


void AMoyoCharacter::SlingDown()
{
	bSlingHeld = true;
}

void AMoyoCharacter::SlingUp()
{
	bSlingHeld = false;

	// Launch player
	slingDir.X = 0.f;
	LaunchCharacter(slingDir * slingMag, false, false);
}


bool AMoyoCharacter::CanJumpInternal_Implementation() const
{
	return true;
	//return CanJumpInternal();
}

bool AMoyoCharacter::LinePlaneIntersection(const FVector& planePoint, const FVector& planeNormal, const FVector& linePoint, const FVector& lineDirection, FVector& result)
{
	if (FVector::DotProduct(planeNormal, lineDirection) == 0)
	{
		return false;
	}

	float t = (FVector::DotProduct(planeNormal, planePoint) - FVector::DotProduct(planeNormal, linePoint)) / FVector::DotProduct(planeNormal, lineDirection);
	result = linePoint + lineDirection * t;
	return true;
}
