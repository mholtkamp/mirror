// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "HeroMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MIRROR_API UHeroMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:

    float m_fSpeed = 0.0f;
    void* m_pMesh  = 0;
	
	
};
