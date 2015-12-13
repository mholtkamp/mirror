// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "MenuGameMode.h"
#include "MenuPawn.h"

AMenuGameMode::AMenuGameMode()
{
    DefaultPawnClass = AMenuPawn::StaticClass();
}

