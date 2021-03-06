// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FireSpitter.generated.h"

UCLASS()
class MIRROR_API AFireSpitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireSpitter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    void FireBullet();

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* m_pParticle;

    UPROPERTY(EditAnywhere)
    float m_fBulletSpeed;

    UPROPERTY(EditAnywhere)
    float m_fFireRate;

    float m_fFireTime;

    UPROPERTY(EditAnywhere)
    float m_fAccelerationSpeed;

    FVector m_vOrigin;
	
};
