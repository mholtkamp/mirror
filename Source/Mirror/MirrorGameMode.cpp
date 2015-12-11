// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "MirrorGameMode.h"
#include "Hero.h"

AMirrorGameMode::AMirrorGameMode()
{
    DefaultPawnClass = AHero::StaticClass();
}


