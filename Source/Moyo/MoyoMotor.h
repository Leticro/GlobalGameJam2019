// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "MoyoMotor.generated.h"

UCLASS()
class UMoyoMotor : public UActorComponent
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

	UPROPERTY()
	TArray<FMoyoSurface> stack;

	// Movement Parameters
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isCylinder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float speed;


	// Cylinder
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector cylinderFocus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float cylinderRadius;

	// Line
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector lineStartPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector lineEndPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector lineDirection;
};
