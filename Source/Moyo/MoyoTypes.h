// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoyoTypes.generated.h"


UENUM(BlueprintType)
enum class EMoyoGameState : uint8
{
	INTRO,
	DIALOG,
	PLAY,
	DIED,
	FINISH
};


USTRUCT(BlueprintType)
struct FMoyoCylinder
{
	GENERATED_BODY()

	FMoyoCylinder()
	{
		isValid = false;
		center = FVector::ZeroVector;
		radius = -1.0f;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isValid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float radius;
};


USTRUCT(BlueprintType)
struct FMoyoLinear
{
	GENERATED_BODY()

	FMoyoLinear()
	{
		isValid = false;
		start = FVector::ZeroVector;
		end = FVector::ZeroVector;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isValid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector end;
};