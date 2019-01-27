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

#include "Interactable.h"
#include "InventoryItem.h"

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

    // Create the pickup collection sphere
    CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
    CollectionSphere->SetupAttachment(RootComponent);
    CollectionSphere->SetSphereRadius(200.f);

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
	defaultGravityScale = MoyoCharMovementComp->GravityScale;
	gravityScaleTarget = defaultGravityScale;
    
    // Initialize Cylinder movement (TEMP)
    FVector position = GetActorLocation();
    
    centerPosition = FVector(1200.0f, 20.0f, 0.0f);
    radiusLength = (position - centerPosition).Size();
    speed = 24.0f;
    cameraDistance = 75.0f;
}

// Called every frame
void AMoyoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    // Restore radius
    FVector location = GetActorLocation();
    FVector elevation = FVector(0.0f, 0.0f, location.Z);
    location.Z = 0.0f;
    FVector currentRadius = location - centerPosition;
    
    currentRadius.Normalize();
    
    SetActorLocation(centerPosition + elevation + currentRadius * radiusLength);
    
    // Set Camera Position/Rotation
    CameraBoom->SetWorldRotation((-currentRadius).Rotation());

	//UE_LOG(LogTemp, Warning, TEXT("MoyoCharacter: Tick was called"));
	if (bSlingHeld)
	{
		SlingUpdateTrajectory(DeltaTime);
	}

	GlideUpdate(DeltaTime);

    CheckForInteractables();
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

void AMoyoCharacter::GlideUpdate(float DeltaTime)
{
	//UKismetMathLibrary::FloatSpringInterp(MoyoCharMovementComp->GravityScale, gravityScaleTarget, hoverSpringState, 100.f, 0.0f, DeltaTime);
	if (MoyoCharMovementComp->Velocity.Z < 0.f)
	{
		MoyoCharMovementComp->GravityScale = gravityScaleTarget;
	}
	else
	{
		MoyoCharMovementComp->GravityScale = defaultGravityScale;
	}
	UE_LOG(LogTemp, Warning, TEXT("GravityScale: %f"), MoyoCharMovementComp->GravityScale);
}


void AMoyoCharacter::CheckForInteractables()
{
    //// Create a LineTrace to check for a hit
    //FHitResult HitResult;

    //int32 Range = 500;
    //FVector StartTrace = FollowCamera->GetComponentLocation();
    //FVector EndTrace = (FollowCamera->GetForwardVector() * Range) + StartTrace;

    //FCollisionQueryParams QueryParams;
    //QueryParams.AddIgnoredActor(this);

    //AMoyoCharacter* IController = Cast<AMoyoCharacter>(GetController());

    //if(IController)
    //{
    //    // Check if something is hit
    //    if(GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
    //    {
    //        // Cast the actor to AInteractable
    //        AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());
    //        // If the cast is successful
    //        if(Interactable)
    //        {
    //            IController->CurrentInteractable = Interactable;
    //            return;
    //        }
    //    }

    //    IController->CurrentInteractable = nullptr;
    //}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMoyoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Sling", IE_Pressed, this, &AMoyoCharacter::SlingDown);
    PlayerInputComponent->BindAction("Sling", IE_Released, this, &AMoyoCharacter::SlingUp);
	PlayerInputComponent->BindAction("Glide", IE_Pressed, this, &AMoyoCharacter::GlideDown);
	PlayerInputComponent->BindAction("Glide", IE_Released, this, &AMoyoCharacter::GlideUp);

    PlayerInputComponent->BindAxis("MoveRight", this, &AMoyoCharacter::MoveRight);
}

void AMoyoCharacter::MoveRight(float Value)
{
    // Find new movement direction
    
    FVector location = GetActorLocation();
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


void AMoyoCharacter::GlideDown()
{
	gravityScaleTarget = glideGravityScale;
}

void AMoyoCharacter::GlideUp()
{
	gravityScaleTarget = defaultGravityScale;
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
