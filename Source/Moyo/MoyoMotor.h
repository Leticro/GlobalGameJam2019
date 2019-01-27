// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
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

	UFUNCTION(BlueprintPure)
	FVector GetForwardVector(float speed) const;

	UFUNCTION(BlueprintPure)
	float GetForwardScalar(float speed) const;

	UPROPERTY()
	TArray<FMoyoSurface> stack;

	// Movement Parameters
	UPROPERTY(VisibleAnywhere)
	bool isCylinder;


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


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool onlyOneSurface;


	UPROPERTY(VisibleAnywhere)
	bool locked;


};
