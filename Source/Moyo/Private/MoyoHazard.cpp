// Fill out your copyright notice in the Description page of Project Settings.

#include "MoyoHazard.h"
#include "MoyoCharacter.h"

// Sets default values for this component's properties
UMoyoHazard::UMoyoHazard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Cap"));
	collider->SetCapsuleSize(200.0f, 200.0f, 1.0f);
	collider->AttachToComponent(this, rules);
}


// Called when the game starts
void UMoyoHazard::BeginPlay()
{
	Super::BeginPlay();

	collider->OnComponentBeginOverlap.AddDynamic(this, &UMoyoHazard::OnBeginOverlap);
	
}


// Called every frame
void UMoyoHazard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoyoHazard::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMoyoCharacter* Other = Cast<AMoyoCharacter>(OtherActor);
	Other->DoDeath();
}
