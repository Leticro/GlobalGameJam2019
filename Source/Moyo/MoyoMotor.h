// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "MoyoTypes.h"
#include "MoyoMotor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOYO_API UMoyoMotor : public UActorComponent
{
	GENERATED_BODY()

	UMoyoMotor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AssignSurface(FMoyoSurface InSurface);

	UFUNCTION(BlueprintCallable)
	void RemoveSurface(FMoyoSurface surface);

	UFUNCTION(BlueprintCallable)
	void ClampToCylinder();

	UFUNCTION(BlueprintCallable)
	FVector GetForwardVector(float speed);

	UFUNCTION(BlueprintPure)
	float GetForwardScalar(float speed) const;

	UPROPERTY()
	TArray<FMoyoSurface> stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fixedRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoyoMotorState motorState;

	// owmer
	UPROPERTY(VisibleAnywhere)
	AActor* ownerActor;

	// Cylinder
	UPROPERTY(VisibleAnywhere)
	FVector cylinderFocus;
	UPROPERTY(VisibleAnywhere)
	float cylinderRadius;

	// Line
	UPROPERTY(VisibleAnywhere)
	FVector lineStartPoint;
	UPROPERTY(VisibleAnywhere)
	FVector lineEndPoint;

	UPROPERTY(VisibleAnywhere)
	FVector lineDirection;

	UPROPERTY(VisibleAnywhere)
	FVector boundDirection;

	UPROPERTY(VisibleAnywhere)
	FVector lastGoodPosition;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isLinearBound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool onlyOneSurface;


	UPROPERTY(VisibleAnywhere)
	bool locked;


};
