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
	boundDirection = lineDirection;
	Super::BeginPlay();

    
}


void UMoyoMotor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

double UMoyoMotor::GetPolarR()
{
	if (fixedRadius > 1.0f)
	{
		return fixedRadius;
	}
	const FVector currentRay = ownerActor->GetActorLocation() - cylinderFocus;

	return FMath::Lerp(currentRay.Size2D(), cylinderRadius, 0.1f);
}
double UMoyoMotor::GetPolarPhi()
{
	const FVector currentRay = ownerActor->GetActorLocation() - cylinderFocus;
	return FMath::Atan2((double)currentRay.Y, (double)currentRay.X);
}

FVector UMoyoMotor::GetCartesian(double R, double Phi)
{
	const float elevation = fixedElevation < 1.0f ? ownerActor->GetActorLocation().Z : fixedElevation;
	const double x = R * FMath::Cos(Phi);
	const double y = R * FMath::Sin(Phi);
	return FVector(x + cylinderFocus.X, y + cylinderFocus.Y, elevation);
}



FVector UMoyoMotor::GetLinearVector(float MoveSpeed)
{
	FVector location = ownerActor->GetActorLocation();
	const float elevation = fixedElevation < 1.0f ? ownerActor->GetActorLocation().Z : fixedElevation;
	location.Z = 0.0f;

	switch(motorState)
	{
	case EMoyoMotorState::LINEAR:
		if (isLinearBound)
		{
			lineStartPoint.Z = 0.0f;
			lineEndPoint.Z = 0.0f;
			const float lineLength = FVector::Dist2D(lineEndPoint, lineStartPoint);
			const float toStart = FVector::Dist2D(location, lineStartPoint);
			const float toEnd = FVector::Dist2D(location, lineEndPoint);
			if (toStart > lineLength)
			{
				FVector mostly = FMath::Lerp(lineEndPoint, lineStartPoint, 0.1f);
				ownerActor->SetActorLocation(FVector(mostly.X, mostly.Y, elevation));

				boundDirection = lineStartPoint - location;
				boundDirection.Normalize();

			}
			else if (toEnd > lineLength)
			{
				const FVector mostly = FMath::Lerp(lineStartPoint, lineEndPoint, 0.1f);
				ownerActor->SetActorLocation(FVector(mostly.X, mostly.Y, elevation));
				boundDirection = lineEndPoint - location;
				boundDirection.Normalize();

			}
			return -boundDirection;
		}
		else
		{
			const FVector point = location;
			const FVector origin = FVector(lineStartPoint.X, lineStartPoint.Y, 0.0f);
			const FVector closestPoint = origin + (lineDirection * ((point - origin) | lineDirection));
			float missedLength;
			FVector missedDir;
			(closestPoint - point).ToDirectionAndLength(missedDir, missedLength);
			if (MoveSpeed > 0)
			{
				missedDir *= -1.0f;
			}
			if (missedLength > 30.0f)
			{
				return FMath::Lerp(lineDirection, missedDir, 0.3f);
			}
			return FMath::Lerp(lineDirection, missedDir, 0.1f);
			

		}
	default:
		return ownerActor->GetActorForwardVector();
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
