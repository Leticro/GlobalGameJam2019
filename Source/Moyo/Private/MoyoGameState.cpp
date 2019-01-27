// Fill out your copyright notice in the Description page of Project Settings.

#include "MoyoGameState.h"

AMoyoGameState::AMoyoGameState()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("DataTable'/Game/Data/ItemDB.ItemDB'"));
    ItemDB = BP_ItemDB.Object;
}

UDataTable* AMoyoGameState::GetItemDB() const
{
    return ItemDB;
}