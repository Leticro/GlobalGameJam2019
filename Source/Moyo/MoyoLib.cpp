// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#include "MoyoLib.h"
#include "MoyoMotor.h"
#include "Moyo.h"

FVector UMoyoLib::GetMoveDestination(float DeltaTime, AActor* Actor, float MoveSpeed, UMoyoMotor* Motor)
{
	Motor->ClampToCylinder();
	float Scalar = Motor->GetForwardScalar(DeltaTime*MoveSpeed);
	FVector MoveDelta = Motor->GetForwardVector(MoveSpeed);
	FVector Last = Actor->GetActorLocation();
	return Last + MoveDelta*Scalar;
}