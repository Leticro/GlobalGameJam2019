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
    
    // Hardcode to "Start to Island01"
	isCylinder = false;
    lineStartPoint = FVector(4200.0f, 1120.0f, 0.0f);
    lineEndPoint = FVector(-20.0f, 1120.0f, 0.0f);
    
    lineDirection = lineEndPoint - lineStartPoint;
    lineDirection.Z = 0.0f;
    lineDirection.Normalize();
}


void UMoyoMotor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UMoyoMotor::AssignSurface(FMoyoSurface surface)
{
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
}

void UMoyoMotor::RemoveSurface(FMoyoSurface surface)
{
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
