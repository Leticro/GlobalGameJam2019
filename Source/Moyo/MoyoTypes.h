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

USTRUCT(BlueprintType)
struct FMoyoSurface
{
	GENERATED_BODY()

	FMoyoSurface()
	{
		priority = -1;
		isCylinder = true;
		center = FVector::ZeroVector;
		radius = 1000.0f;
		start = FVector::ZeroVector;
		end = FVector::ZeroVector;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 priority;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool isCylinder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float radius;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector end;
};
