// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "MoyoTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "MoyoGuy.generated.h"

class UMoyoCharacterMovementComponent;
class UMoyoMotor;

UCLASS(config=Game)
class AMoyoGuy : public ACharacter
{
	GENERATED_BODY()

public:
	AMoyoGuy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Custom Character Movement Component
	UMoyoCharacterMovementComponent* MoyoCharMovementComp;



	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	void MoveRightCylinder(float Val);
	void MoveRightLinear(float Val);



public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMoyoMotor* motor;

protected:
	UPROPERTY()
	float speed;
	UPROPERTY()
	float inputDir;
	UPROPERTY()
	float defaultGravityScale;


public:
	AMoyoGuy();

};
