// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "MoyoGameState.generated.h"

/**
 * 
 */
UCLASS()
class MOYO_API AMoyoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    AMoyoGameState();

    UDataTable* GetItemDB() const;

protected:
    UPROPERTY(EditDefaultsOnly)
        class UDataTable* ItemDB;
};
