// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MoyoGameMode.h"
#include "MoyoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMoyoGameMode::AMoyoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MoyoCharacterBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
