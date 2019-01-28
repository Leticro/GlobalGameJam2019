// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoCharacter.h"
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

#include "Interactable.h"
#include "InteractableComponent.h"
#include "Pickup.h"
#include "InventoryItem.h"

AMoyoCharacter::AMoyoCharacter(const FObjectInitializer& ObjectInitializer) 
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UMoyoCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMoyoCharacterMovementComponent>(CharacterMovementComponentName))
{
	
	// rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SocketOffset = FVector(0.f, 0.f, 80.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);


    // Create the pickup collection sphere
    CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
    CollectionSphere->SetupAttachment(RootComponent);
    CollectionSphere->SetSphereRadius(CollectionSphereRadius);

	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


void AMoyoCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoyoPlayerController = (AMoyoPlayerController*) GetWorld()->GetFirstPlayerController();
    MoyoPlayerController->CurrentPawn = this;


	// Temporary measure to be able to see cursor
	//FInputModeGameAndUI defaultInputMode;
	//defaultInputMode.SetHideCursorDuringCapture(false);
	//MoyoPlayerController->SetInputMode(defaultInputMode);

	bSlingHeld = false;
	defaultGravityScale = MoyoCharMovementComp->GravityScale;
	gravityScaleTarget = defaultGravityScale;
	
	hoverSpringState.Velocity = 0.f;
	hoverSpringState.PrevError = 0.f;

    speed = 24.0f;
	
	inputDir = 1.0f;
    
	dashDirection = inputDir;
    

	// TEMP - Keeping for reference

	// Start to Island 01
	//SetLine(FVector(4020.0f, 1120.0f, 0.0f), FVector(-20.0f, 1120.0f, 0.0f));

	// Island 01
	//SetCylinder(FVector(0.0f, 0.0f, 0.0f), 1120.0f);

	// Island 01 to Island 02
	//SetLine(FVector(-792.0f, 792.0f, 0.0f), FVector(-4200.0f, -3000.0f, 0.0f));

	// Island 02
	//SetCylinder(FVector(-3200.0f, -4000.0f, 0.0f), 1414.0f);

	// Island 02 to Island 03
	//SetLine(FVector(4800.0f, -3600.0f, 0.0f), FVector(-3600.0f, -2800.0f, 0.0f));

	// Island 03
	//SetCylinder(FVector(4500.0f, -4500.0f, 0.0f), 1120.0f);

	// Island 03 to Island 04
	//SetLine(FVector(3910.0f, -10.0f, 0.0f), FVector(3370.0f, -4380.0f, 0.0f));

	// Island 04
	//SetCylinder(FVector(4900.0f, 0.0f, 0.0f), 1000.0f);
}

// Called every frame
void AMoyoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Correct location if outside rails
    FVector location = GetActorLocation();
    location.Z = 0.0f;

	if (!CameraBoom)
	{
		return;

	}

	if (gravityScaleTarget < 0.9f*defaultGravityScale)
	{
		floatRemaining -= DeltaTime;
		if (floatRemaining < 0)
		{
			floatRemaining = 7.5f;
			DoKeepFloat();
		}
	}
	switch (motor->motorState)
	{
	case EMoyoMotorState::CYLINDER:
	{
		// Character Position
		FVector currentRadius = location - motor->cylinderFocus;
		currentRadius.Normalize();
		// Camera Position/Rotation
		//currentRadius.Z = -0.1f;
		//currentRadius.Normalize();
		CameraBoom->SetWorldRotation((-currentRadius).Rotation());
		break;
	}
	case EMoyoMotorState::LINEAR:
	{
		FVector cameraDir = FVector::CrossProduct(motor->lineDirection, FVector(0.0f, 0.0f, 1.0f));

		CameraBoom->SetWorldRotation((-cameraDir).Rotation());
		break;
	}
	default:
		break;
	}

	//UE_LOG(LogTemp, Warning, TEXT("MoyoCharacter: Tick was called"));
	if (bSlingHeld)
	{
		//Disabled for now 
		//SlingUpdateTrajectory(DeltaTime);
	}
	
	GlideUpdate(DeltaTime);

	DashUpdate(DeltaTime);

	CheckForInteractables();
	CheckForInteractableComponents();
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
	//glideAmount = UKismetMathLibrary::FloatSpringInterp(glideAmount, glideIntention, hoverSpringState, 100.f, 0.5f, DeltaTime);
	glideAmount = FMath::FInterpTo(glideAmount, glideIntention, DeltaTime, 10.0f);

	if (glideIntention > 0.1f && MoyoCharMovementComp->Velocity.Z < 0.f)
	{
		//MoyoCharMovementComp->GravityScale = UKismetMathLibrary::FloatSpringInterp(MoyoCharMovementComp->GravityScale, gravityScaleTarget, hoverSpringState, 100.f, 0.5f, DeltaTime);
		MoyoCharMovementComp->GravityScale = (defaultGravityScale - glideGravityScale) * (1.0f - glideAmount) + glideGravityScale;

        float dir = -1.0f;
        if(inputDir < 0.0f) {
            dir = 1.0f;
        }
        
        switch (motor->motorState)
        {
            case EMoyoMotorState::CYLINDER:
            {
                float angle = dir * 10.0f * (180.f / (motor->cylinderRadius * 3.14159265f));
                FVector relativePos = FQuat(FVector(0.f, 0.f, 1.0f), FMath::DegreesToRadians(angle)) * (GetActorLocation() - motor->cylinderFocus);
                SetActorLocation(motor->cylinderFocus + relativePos);
                
                // Just so we "Run" and face the dash direction
                MoveRightCylinder(-dir);
                break;
            }
            case EMoyoMotorState::LINEAR:
            {
                FVector relativePos = motor->lineDirection * 10.0f;
                SetActorLocation(GetActorLocation() + dir * relativePos);
                
                // Just so we "Run" and face the dash direction
                MoveRightLinear(dir * dashDirection);
                break;
            }
            default:
                break;
        }
	}
	else
	{
		MoyoCharMovementComp->GravityScale = defaultGravityScale;
	}
	//UE_LOG(LogTemp, Warning, TEXT("GlideAmount: %f, GravityScale: %f"), glideAmount, MoyoCharMovementComp->GravityScale);
}


void AMoyoCharacter::DashUpdate(float DeltaTime)
{
	if (bDashing)
	{
		float t = (GetWorld()->TimeSeconds - dashStartTime) / dashDuration;
		if (t > 1.f)
		{
			bDashing = false;
		}
		else
		{

			float EaseInOutCurve = FMath::InterpEaseInOut(0.0f, dashDistance, t, dashCurveExponent);
			float EaseOutCurve = FMath::InterpEaseOut(0.0f, dashDistance, t, dashCurveExponent);
			float outerT = FMath::Clamp(t * 4.0f, 0.f, 1.f);
			float dashNextPos = FMath::Lerp(EaseInOutCurve, EaseOutCurve, outerT);

            float dir = -1.0f;
            if(inputDir < 0.0f) {
                dir = 1.0f;
            }

			//float dashNextPos = FMath::InterpEaseInOut(0.0f, dashDistance, t, dashCurveExponent);
			float dashPosDelta = (dashNextPos - dashPrevPos) * dashDirection;
			//float dashVel = dashPosDelta / DeltaTime;
			//float value = (dashVel * dashDirection) / speed;
			
			switch (motor->motorState)
			{
			case EMoyoMotorState::CYLINDER:
			{
				float angle = -dashPosDelta * (180.f / (motor->cylinderRadius * 3.14159265f));
				FVector relativePos = FQuat(FVector(0.f, 0.f, 1.0f), FMath::DegreesToRadians(angle)) * (GetActorLocation() - motor->cylinderFocus);
				SetActorLocation(motor->cylinderFocus + relativePos);

				// Just so we "Run" and face the dash direction
				MoveRightCylinder(dashDirection);
				break;
			}
            case EMoyoMotorState::LINEAR:
            {
                FVector relativePos = motor->lineDirection * 16.0f;
                SetActorLocation(GetActorLocation() + dir * relativePos);

				// Just so we "Run" and face the dash direction
				MoveRightLinear(dir * dashDirection);
				break;
            }
			default:
				break;
			}

			dashPrevPos = dashNextPos;
		}

	}
	else
	{
		if (!MoyoCharMovementComp->IsFalling())
		{
			bDashAvailable = true;
		}
	}
}


void AMoyoCharacter::CheckForInteractables()
{
    AMoyoPlayerController* IController = Cast<AMoyoPlayerController>(GetController());

    if(IController->Inventory.Num())
    {
        FVector playerLoc = GetActorLocation();
        playerLoc.Z += 100;
        IController->CurrentObject->SetActorLocation(playerLoc);
    }
    else if(IController->CurrentObject)
    {
        FVector forward = GetActorLocation() + CameraBoom->GetForwardVector() * 80.0f;

        IController->CurrentObject->SetActorLocation(forward);
        IController->CurrentObject = nullptr;

    } else
    {
        // Get all overlapping Actors and store them in an array
        TArray<AActor*> CollectedActors;
        CollectionSphere->GetOverlappingActors(CollectedActors);

        // For each collected Actor
        for(int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
        {
            // Cast the actor to AInteractable
            AInteractable* Interactable = Cast<AInteractable>(CollectedActors[iCollected]);
            // If the cast is successful
            if(Interactable)
            {
                IController->CurrentInteractable = Interactable;
                return;
            }
        }
    }
    IController->CurrentInteractable = nullptr;
}


void AMoyoCharacter::CheckForInteractableComponents()
{
	AMoyoPlayerController* IController = Cast<AMoyoPlayerController>(GetController());

	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// For each collected Actor
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		
		UActorComponent * OtherComponent = CollectedActors[iCollected]->GetComponentByClass(UInteractableComponent::StaticClass());
		if (OtherComponent)
		{
			UInteractableComponent* Other = Cast<UInteractableComponent>(OtherComponent);
			IController->CurrentInteractableComponent = Other;
			return;
		}
	}
	IController->CurrentInteractableComponent = nullptr;
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
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMoyoCharacter::DashDown);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMoyoCharacter::DashUp);

    PlayerInputComponent->BindAxis("MoveRight", this, &AMoyoGuy::MoveRight);
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
	glideIntention = 1.0f;
}

void AMoyoCharacter::GlideUp()
{
	gravityScaleTarget = defaultGravityScale;
	glideIntention = 0.0f;
}

void AMoyoCharacter::DashDown()
{
	// We can only dash while in mid air, and only once per jump
	if(bDashAvailable && MoyoCharMovementComp->IsFalling())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Dash initiated"));
		dashStartTime = GetWorld()->TimeSeconds;
		dashPrevPos = 0.f;
		bDashing = true;
		dashDirection = inputDir;
		bDashAvailable = false;
	}
}

void AMoyoCharacter::DashUp()
{

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
