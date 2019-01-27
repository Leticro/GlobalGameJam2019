// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "MoyoTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "MoyoGuy.generated.h"

class UMoyoCharacterMovementComponent;
class UMoyoMotor;

UCLASS()
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


	UFUNCTION(BlueprintNativeEvent)
	void DoFallOut();
	virtual void DoFallOut_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void DoDeath();
	virtual void DoDeath_Implementation();



public:

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Moyo")
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


public:
	AMoyoGuy();

};
