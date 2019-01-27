// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "MoyoTypes.h"
#include "MoyoVolume.generated.h"

UCLASS()
class AMoyoVolume : public AActor
{
	GENERATED_BODY()

public:
	AMoyoVolume(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* root;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* collider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* anchorLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* anchorRight;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBillboardComponent* iconLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBillboardComponent* iconRight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMoyoCylinder cylinder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMoyoLinear linear;
};
