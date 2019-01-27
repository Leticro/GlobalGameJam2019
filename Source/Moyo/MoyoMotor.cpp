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
	Super::BeginPlay();
    
}


void UMoyoMotor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMoyoMotor::ClampToCylinder()
{
	if (isCylinder)
	{
		FVector location = GetOwner()->GetActorLocation();
		const FVector elevation = FVector(0.0f, 0.0f, location.Z);
		location.Z = 0.0f;
		FVector currentRadius = location - cylinderFocus;
		currentRadius.Z = 0.0f;
		currentRadius.Normalize();

		FVector end = cylinderFocus + elevation + currentRadius * cylinderRadius;
		GetOwner()->SetActorLocation(end);

	}
}

FVector UMoyoMotor::GetForwardVector(float input) const
{

	FVector location = GetOwner()->GetActorLocation();
	location.Z = 0.0f;
	const FVector radius = location - cylinderFocus;
	const FVector newRadius = radius.RotateAngleAxis(input, FVector(0.0f, 0.0f, 1.0f));

	FVector tangent = newRadius - radius;
	tangent.Z = 0;
	tangent.Normalize();
	return tangent;
}

float UMoyoMotor::GetForwardScalar(float input) const
{
	return 10.0f * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(input / 2.0f)));
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

 	isCylinder = stack[0].isCylinder;

	cylinderFocus = stack[0].center;
	cylinderRadius = stack[0].radius;
	lineStartPoint = stack[0].start;
	lineEndPoint = stack[0].end;
	if(!isCylinder)
	{
		lineDirection = lineEndPoint - lineStartPoint;
		lineDirection.Z = 0.0f;
		lineDirection.Normalize();
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
		isCylinder = stack[0].isCylinder;

		cylinderFocus = stack[0].center;
		cylinderRadius = stack[0].radius;
		lineStartPoint = stack[0].start;
		lineEndPoint = stack[0].end;
		if (!isCylinder)
		{
			lineDirection = lineEndPoint - lineStartPoint;
			lineDirection.Z = 0.0f;
			lineDirection.Normalize();
		}
	}
}
