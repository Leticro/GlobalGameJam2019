// Fill out your copyright notice in the Description page of Project Settings.

//#include "MoyoCharacterMovementComponent.h"
#include "Moyo/Public/MoyoCharacterMovementComponent.h"
#include "GameFramework/Character.h"

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