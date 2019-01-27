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
	void OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor);


	UFUNCTION()
	void OnEndOverlap(AActor* MyOverlappedActor, AActor* OtherActor);


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* collider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AActor>> acceptedClasses;

	UPROPERTY(VisibleAnywhere)
	FMoyoSurface surfacedata;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 priority;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector lineStart;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector lineEnd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float cylinderRadius;
};
