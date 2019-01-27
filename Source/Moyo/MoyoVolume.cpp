// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoVolume.h"
#include "MoyoMotor.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "MoyoCharacter.h"

AMoyoVolume::AMoyoVolume(const FObjectInitializer& ObjectInitializer) 
{
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Cap"));
	collider->SetCapsuleSize(800.0f,800.0f,1.0f);
	collider->AttachToComponent(root, rules);
}
	

void AMoyoVolume::BeginPlay()
{
	surfacedata.priority = priority;
	if (cylinderRadius > 1.0f)
	{
		surfacedata.motorState = EMoyoMotorState::CYLINDER;
	}
	else
	{
		surfacedata.motorState = EMoyoMotorState::LINEAR;
	}
	
	surfacedata.center = collider->GetComponentLocation();
	surfacedata.center.Z = 0.0f;
	surfacedata.radius = cylinderRadius;
	FTransform t = collider->GetComponentToWorld();
	surfacedata.start = lineStart;
	surfacedata.end = lineEnd;

	TArray<AActor*> overlaps;
	GetOverlappingActors(overlaps);
	for (auto x : overlaps)
	{
		OnBeginOverlap(this, x);
	}

	OnActorBeginOverlap.AddDynamic(this, &AMoyoVolume::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMoyoVolume::OnEndOverlap);
}

void AMoyoVolume::OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor && acceptedClasses.Contains(OtherActor->GetClass()))
	{
		UActorComponent* OtherMotorComponent = OtherActor->GetComponentByClass(UMoyoMotor::StaticClass());
		if (OtherMotorComponent)
		{
			UMoyoMotor* OtherMotor = Cast<UMoyoMotor>(OtherMotorComponent);
			OtherMotor->AssignSurface(surfacedata);
		}
	}
}

void AMoyoVolume::OnEndOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor && acceptedClasses.Contains(OtherActor->GetClass()))
	{
		UActorComponent* OtherMotorComponent = OtherActor->GetComponentByClass(UMoyoMotor::StaticClass());
		if (OtherMotorComponent)
		{
			UMoyoMotor* OtherMotor = Cast<UMoyoMotor>(OtherMotorComponent);
			OtherMotor->RemoveSurface(surfacedata);
		}
	}
}
