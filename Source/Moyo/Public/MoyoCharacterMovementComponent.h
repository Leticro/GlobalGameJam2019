// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoyoCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOYO_API UMoyoCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	
	virtual bool DoJump(bool bReplayingMoves) override;

};
