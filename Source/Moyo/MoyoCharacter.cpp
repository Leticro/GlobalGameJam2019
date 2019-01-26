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

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

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
    
    // Initialize Cylinder movement
    FVector position = GetActorLocation();
    
    centerPosition = FVector(1200.0f, 20.0f, 0.0f);
    radiusLength = (position - centerPosition).Size();
    speed = 24.0f;
}

// Called every frame
void AMoyoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMoyoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMoyoCharacter::TouchStopped);
}

void AMoyoCharacter::MoveRight(float Value)
{
    // Restore radius
    FVector location = GetActorLocation();
    FVector elevation = FVector(0.0f, 0.0f, location.Z);
    location.Z = 0.0f;
    FVector currentRadius = location - centerPosition;
    
    currentRadius.Normalize();
    
    SetActorLocation(centerPosition + elevation + currentRadius * radiusLength);
    
    // Find new movement direction
    
    float angle = speed * Value;
    
    FVector radius = location - centerPosition;
    
    FVector newRadius = radius.RotateAngleAxis(angle, FVector(0.0f, 0.0f, 1.0f));
    
    FVector tangent = newRadius - radius;
    tangent.Normalize();
    
    float distance = 10.0f * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(angle/2.0f)));
    
	// add movement in that direction
	AddMovementInput(-tangent, distance);
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
