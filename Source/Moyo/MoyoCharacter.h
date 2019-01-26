// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoyoCharacter.generated.h"

class UMoyoCharacterMovementComponent;

UCLASS(config=Game)
class AMoyoCharacter : public ACharacter
{
	GENERATED_BODY()
    
    FVector centerPosition;
    float radiusLength;
    float speed;

public:
	AMoyoCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Custom Character Movement Component
	UMoyoCharacterMovementComponent* MoyoCharMovementComp;


	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	class AMoyoPlayerController* MoyoPlayerController;

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void SlingDown();
	void SlingUp();
	void SlingUpdateTrajectory(float DeltaTime);


	virtual bool CanJumpInternal_Implementation() const override;

	bool LinePlaneIntersection(const FVector& planePoint, const FVector& planeNormal, const FVector& linePoint, const FVector& lineDirection, FVector& result);


public:
	UPROPERTY(EditAnywhere)
	float minSlingRadius = 50.f;
	UPROPERTY(EditAnywhere)
	float maxSlingRadius = 200.f;
	UPROPERTY(EditAnywhere)
	float minSlingVelocity = 50.f;
	UPROPERTY(EditAnywhere)
	float maxSlingVelocity = 200.f;



protected:

	// Sling Parameters
	bool bSlingHeld;
	FVector slingDir;
	float slingMag;


public:
	AMoyoCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
