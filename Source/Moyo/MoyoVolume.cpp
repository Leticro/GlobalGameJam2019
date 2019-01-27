// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoVolume.h"
#include "MoyoMotor.h"
#include "UObject/ConstructorHelpers.h"
#include "MoyoCharacter.h"

AMoyoVolume::AMoyoVolume(const FObjectInitializer& ObjectInitializer) 
{
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	anchorLeft = CreateDefaultSubobject<USceneComponent>(TEXT("Left"));
	anchorLeft->AttachToComponent(root, rules);
	anchorLeft->SetRelativeLocation(FVector::ForwardVector*100.0f);
	iconLeft = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconLeft"));
	iconLeft->AttachToComponent(anchorLeft, rules);

	anchorRight = CreateDefaultSubobject<USceneComponent>(TEXT("Right"));
	anchorRight->AttachToComponent(root, rules);
	anchorRight->SetRelativeLocation(-FVector::ForwardVector*100.0f);
	iconRight = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconRight"));
	iconRight->AttachToComponent(anchorRight, rules);


	if (iconLeft != nullptr)
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_Info;
			FText NAME_Info;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/S_KPrismatic"))
				, ID_Info(TEXT("Info"))
				, NAME_Info(NSLOCTEXT("SpriteCategory", "Info", "Info"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		iconLeft->Sprite = ConstructorStatics.SpriteTexture.Get();
		iconLeft->SpriteInfo.Category = ConstructorStatics.ID_Info;
		iconLeft->SpriteInfo.DisplayName = ConstructorStatics.NAME_Info;
		iconLeft->bIsScreenSizeScaled = true;


		iconRight->Sprite = ConstructorStatics.SpriteTexture.Get();
		iconRight->SpriteInfo.Category = ConstructorStatics.ID_Info;
		iconRight->SpriteInfo.DisplayName = ConstructorStatics.NAME_Info;
		iconRight->bIsScreenSizeScaled = true;

	}

}
	

void AMoyoVolume::BeginPlay()
{
	TArray<AActor*> AllActors;
	GetOverlappingActors(AllActors);
	for (auto x : AllActors)
	{
		OnOverlap(this, x);
	}
}

void AMoyoVolume::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	UActorComponent* OtherMotorComponent = OtherActor->GetComponentByClass(UMoyoMotor::StaticClass());
	if (OtherMotorComponent)
	{
		UMoyoMotor* OtherMotor = Cast<UMoyoMotor>(OtherMotorComponent);
		OtherMotor->AssignSurface(cylinder, linear);
	}
}