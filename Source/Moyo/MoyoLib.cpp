// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#include "MoyoLib.h"
#include "MoyoMotor.h"
#include "Moyo.h"

FVector UMoyoLib::GetMoveDestination(float DeltaTime, AActor* Actor, float MoveSpeed, UMoyoMotor* Motor)
{
	if (Motor->motorState == EMoyoMotorState::CYLINDER)
	{
		const FVector start = Actor->GetActorLocation();
		const double PolarR = Motor->GetPolarR();
		double PolarPhi = Motor->GetPolarPhi();
		PolarPhi += (double)MoveSpeed*DeltaTime;
		const FVector result = Motor->GetCartesian(PolarR, PolarPhi);
		if (MoveSpeed < 0.001f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Polar %.2f %.2f"), (float)PolarR, (float)PolarPhi);
		}
		return result;
	}
	else
	{
		const FVector start = Actor->GetActorLocation();
		return start - Motor->GetLinearVector(MoveSpeed)*MoveSpeed;
	}
	
}