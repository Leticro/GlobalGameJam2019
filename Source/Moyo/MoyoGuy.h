// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Moyo.h"
#include "MoyoTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "MoyoGuy.generated.h"

class UMoyoCharacterMovementComponent;
class UMoyoMotor;

UCLASS()
class AMoyoGuy : public ACharacter
{
	GENERATED_BODY()

public:
	AMoyoGuy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Custom Character Movement Component
	UMoyoCharacterMovementComponent* MoyoCharMovementComp;



	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintNativeEvent)
	void DoDash();
	virtual void DoDash_Implementation() { return; }


	UFUNCTION(BlueprintImplementableEvent)
	void DoKeepFloat();

	UFUNCTION(BlueprintImplementableEvent)
	void DoKeepWalking();


	UFUNCTION(BlueprintImplementableEvent)
	void DoPickup();

	UFUNCTION(BlueprintImplementableEvent)
	void DoGreeting();

    UFUNCTION(BlueprintImplementableEvent)
    void DoRecieve(FName itemID);

	UFUNCTION(BlueprintImplementableEvent)
	void DoDrop();


	UFUNCTION(BlueprintNativeEvent)
	void DoFallOut();
	virtual void DoFallOut_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void DoDeath();
	virtual void DoDeath_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Moyo")
        void SendGreeting() { DoGreeting(); }

	UFUNCTION(BlueprintCallable, Category = "Moyo")
		void SendItem(FName itemID);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FName> RemainingItems;
public:

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Moyo")
	void MoveRight(float Val);

	void MoveRightCylinder(float Val);
	void MoveRightLinear(float Val);



public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMoyoMotor* motor;

protected:
	UPROPERTY()
	float speed;
	UPROPERTY()
	float inputDir;

	UPROPERTY()
	float floatRemaining;

	UPROPERTY()
	float footRemaining;


public:
	AMoyoGuy();

};
