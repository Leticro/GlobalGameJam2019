// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoyoLib.generated.h"

UCLASS()
class UMoyoLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	
	UFUNCTION(BlueprintCallable)
	static FVector GetMoveDestination(float DeltaTime, AActor* Actor, float MoveSpeed, UMoyoMotor* Motor);
};
