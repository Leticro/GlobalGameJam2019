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


	double GetPolarR();
	double GetPolarPhi();
	FVector GetCartesian(double R, double Phi);
	FVector GetLinearVector(float MoveSpeed);


	UPROPERTY()
	TArray<FMoyoSurface> stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fixedRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fixedElevation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoyoMotorState motorState;

	// owmer
	UPROPERTY(VisibleAnywhere)
	AActor* ownerActor;

	// Cylinder
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector cylinderFocus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cylinderRadius;

	// Line
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lineStartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lineEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lineDirection;

	UPROPERTY(VisibleAnywhere)
	FVector boundDirection;

	UPROPERTY(VisibleAnywhere)
	FVector lastGoodPosition;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isLinearBound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool onlyOneSurface;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool locked;


};
