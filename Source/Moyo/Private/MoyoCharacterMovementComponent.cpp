// Fill out your copyright notice in the Description page of Project Settings.

//#include "MoyoCharacterMovementComponent.h"
#include "Moyo/Public/MoyoCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"

void UMoyoCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	

}


void UMoyoCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	

}

bool UMoyoCharacterMovementComponent::DoJump(bool bReplayingMoves)
{

	//UE_LOG(LogTemp, Warning, TEXT("Do Jump Called on my derived class"));

	if (CharacterOwner && CharacterOwner->CanJump())
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = FMath::Max(Velocity.Z, JumpZVelocity);
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	return false;
}

bool UMoyoCharacterMovementComponent::IsNearingGround(float nearDistance) const
{
	if (IsFalling())
	{
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, GetOwner());
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		FVector Start = GetOwner()->GetActorLocation();
		FVector End = Start + FVector(0.f, 0.f, 1.f) * -500.0f;

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,     //result
			Start,		//start
			End,		//end
			ECollisionChannel::ECC_WorldStatic, //collision channel
			RV_TraceParams
		);

		if (RV_Hit.bBlockingHit)
		{
			float dist = (RV_Hit.ImpactPoint - Start).Size();
			return dist < nearDistance;
		}

	}
	return false;
}