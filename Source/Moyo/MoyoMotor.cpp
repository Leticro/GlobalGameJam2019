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
	isCylinder = true;
	cylinderFocus = FVector(-3200.0f, -4000.0f, 0.0f),
	cylinderRadius = 1414.0f;

	// ...

}


void UMoyoMotor::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UMoyoMotor::AssignSurface(FMoyoCylinder cylinder, FMoyoLinear linear)
{
	if (cylinder.isValid)
	{
		isCylinder = true;

		cylinderFocus = cylinder.center;
		cylinderRadius = cylinder.radius;
	}
	else
	{
		isCylinder = false;

		lineStartPoint = linear.start;
		lineEndPoint = linear.end;
		lineDirection = lineEndPoint - lineStartPoint;
		lineDirection.Z = 0.0f;
		lineDirection.Normalize();
	}
}