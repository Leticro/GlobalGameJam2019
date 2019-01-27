// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoyoTypes.generated.h"


UENUM(BlueprintType)
enum class EMoyoGameState : uint8
{
	NONE,
	INTRO,
	DIALOG,
	PLAY,
	DIED,
	FINISH
};

UENUM(BlueprintType)
enum class EMoyoMotorState : uint8
{
	NONE,
	CYLINDER,
	LINEAR
};


USTRUCT(BlueprintType)
struct FMoyoSurface
{
	GENERATED_BODY()

	FMoyoSurface()
	{
		priority = -1;
		motorState = EMoyoMotorState::CYLINDER;
		center = FVector::ZeroVector;
		radius = 1000.0f;
		start = FVector::ZeroVector;
		end = FVector::ZeroVector;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 priority;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EMoyoMotorState motorState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float radius;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector end;
};
