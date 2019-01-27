// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoMotor.h"
#include "Moyo.h"

UMoyoMotor::UMoyoMotor()
{
	PrimaryComponentTick.bCanEverTick = true;
}




// Called when the game starts
void UMoyoMotor::BeginPlay()
{
	ownerActor = GetOwner();
	lastGoodPosition = ownerActor->GetActorLocation();
	Super::BeginPlay();

    
}


void UMoyoMotor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMoyoMotor::ClampToCylinder()
{	
	switch (motorState)
	{
	case EMoyoMotorState::CYLINDER:
	{
		FVector location = ownerActor->GetActorLocation();
		const FVector elevation = FVector(0.0f, 0.0f, location.Z);
		location.Z = 0.0f;
		FVector currentRay = location - cylinderFocus;
		currentRay.Z = 0.0f;
		currentRay.Normalize();

		float radius = fixedRadius < 1.0f ? cylinderRadius : fixedRadius;
		FVector end = cylinderFocus + elevation + currentRay * radius;
		ownerActor->SetActorLocation(end);
		break;
	}
	case EMoyoMotorState::LINEAR:
	{
		if (!isLinearBound)
		{
			FVector location = ownerActor->GetActorLocation();
			FVector elevation = FVector(0.0f, 0.0f, location.Z);
			location.Z = 0.0f;
			FVector sToP = location - lineStartPoint;
			FVector newLocation = lineDirection * sToP.Size();
			FVector end = lineStartPoint + elevation + newLocation;
			ownerActor->SetActorLocation(end);
		}
		break;
	}
	default:
		break;
	}
}

FVector UMoyoMotor::GetForwardVector(float input)
{
	FVector location = ownerActor->GetActorLocation();
	FVector elevation = FVector(0.0f, 0.0f, location.Z);

	location.Z = 0.0f;
	switch(motorState)
	{
	case EMoyoMotorState::CYLINDER:
	{
		const FVector radius = location - cylinderFocus;
		const FVector newRadius = radius.RotateAngleAxis(input, FVector(0.0f, 0.0f, 1.0f));

		FVector tangent = newRadius - radius;
		tangent.Z = 0;
		tangent.Normalize();
		return tangent;
	}
	case EMoyoMotorState::LINEAR:
		if (isLinearBound)
		{
			const float LineLength = FVector::Dist2D(lineEndPoint, lineStartPoint);
			const float ToStart = FVector::Dist2D(location, lineStartPoint);
			const float ToEnd = FVector::Dist2D(location, lineEndPoint);
			if (ToStart > LineLength)
			{
				const FVector mostly = FMath::Lerp(lineEndPoint, lineStartPoint, 0.1f);
				ownerActor->SetActorLocation(mostly + elevation);

				boundDirection = lineStartPoint - location;
				boundDirection.Normalize();

				location = ownerActor->GetActorLocation();
				location.Z = 0.0f;
			}
			else if (ToEnd > LineLength)
			{
				const FVector mostly = FMath::Lerp(lineStartPoint, lineEndPoint, 0.1f);
				ownerActor->SetActorLocation(mostly + elevation);
				boundDirection = lineEndPoint - location;
				boundDirection.Normalize();

				location = ownerActor->GetActorLocation();
				location.Z = 0.0f;
			}
			return boundDirection;
		}
		return lineDirection;
	default:
		return ownerActor->GetActorForwardVector();
	}
}

float UMoyoMotor::GetForwardScalar(float input) const
{
	switch (motorState)
	{
	case EMoyoMotorState::CYLINDER:
		return 10.0f * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(input / 2.0f)));
	default:
		return input;
	}
}

void UMoyoMotor::AssignSurface(FMoyoSurface surface)
{
	if (locked)
	{
		return;
	}
	stack.Add(surface);
	stack.Sort([](const FMoyoSurface& lhs, const FMoyoSurface& rhs)
	{
		return lhs.priority > rhs.priority;
	});

	motorState = stack[0].motorState;

	cylinderFocus = stack[0].center;
	cylinderRadius = stack[0].radius;
	lineStartPoint = stack[0].start;
	lineStartPoint.Z = 0.0f;
	lineEndPoint = stack[0].end;
	lineEndPoint.Z = 0.0f;
	if(motorState == EMoyoMotorState::LINEAR)
	{
		lineDirection = lineEndPoint - lineStartPoint;
		lineDirection.Normalize();
		boundDirection = lineDirection;
	}
	else if (motorState == EMoyoMotorState::CYLINDER)
	{
		lastGoodPosition = GetOwner()->GetActorLocation();
	}
	if (onlyOneSurface)
	{
		locked = true;
	}
}

void UMoyoMotor::RemoveSurface(FMoyoSurface surface)
{
	if (locked)
	{
		return;
	}
	int32 inpriority = surface.priority;
	stack.RemoveAll([inpriority](const FMoyoSurface& lhs)
	{
		return lhs.priority == inpriority;
	});

 	if (stack.Num() > 0)
	{
		motorState = stack[0].motorState;

		cylinderFocus = stack[0].center;
		cylinderRadius = stack[0].radius;
		lineStartPoint = stack[0].start;
		lineStartPoint.Z = 0.0f;
		lineEndPoint = stack[0].end;
		lineEndPoint.Z = 0.0f;
		if (motorState == EMoyoMotorState::LINEAR)
		{
			lineDirection = lineEndPoint - lineStartPoint;
			lineDirection.Normalize();
		}
	}
}
