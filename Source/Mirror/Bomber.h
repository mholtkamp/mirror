// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bomber.generated.h"

UCLASS()
class MIRROR_API ABomber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomber();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    float m_fSpeed;

    UPROPERTY(EditAnywhere)
    float m_fRange;

    UPROPERTY(EditAnywhere)
    float m_fBulletSpeed;

    UPROPERTY(EditAnywhere)
    float m_fFireTime;

    FVector m_vOrigin;
    float m_fDirection;
};
