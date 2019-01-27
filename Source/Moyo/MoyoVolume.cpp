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
	surfacedata.isCylinder = cylinderRadius > 1.0f;
	
	surfacedata.center = collider->GetComponentLocation();
	surfacedata.center.Z = 0.0f;
	surfacedata.radius = cylinderRadius;
	FTransform t = collider->GetComponentToWorld();
	surfacedata.start = lineStart;
	surfacedata.end = lineEnd;

	OnActorBeginOverlap.AddDynamic(this, &AMoyoVolume::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMoyoVolume::OnEndOverlap);
}

void AMoyoVolume::OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	UActorComponent* OtherMotorComponent = OtherActor->GetComponentByClass(UMoyoMotor::StaticClass());
	if (OtherMotorComponent)
	{
		UMoyoMotor* OtherMotor = Cast<UMoyoMotor>(OtherMotorComponent);
		OtherMotor->AssignSurface(surfacedata);
	}
}

void AMoyoVolume::OnEndOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{

	UActorComponent* OtherMotorComponent = OtherActor->GetComponentByClass(UMoyoMotor::StaticClass());
	if (OtherMotorComponent)
	{
		UMoyoMotor* OtherMotor = Cast<UMoyoMotor>(OtherMotorComponent);
		OtherMotor->RemoveSurface(surfacedata);
	}
}

void AMoyoVolume::Debug()
{

	FTransform t = collider->GetComponentToWorld();
	FVector a = lineStart;
	FVector c = lineEnd;
	DrawDebugDirectionalArrow(GetWorld(), collider->GetComponentLocation(), a, 3.0f, FColor::Red, true, 3.0f);
	DrawDebugDirectionalArrow(GetWorld(), collider->GetComponentLocation(), c, 3.0f, FColor::Green, true, 3.0f);

}
