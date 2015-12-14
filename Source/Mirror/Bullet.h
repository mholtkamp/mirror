// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class MIRROR_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	

    enum ETerminationMode
    {
        TERMINATE_MIN_Z = 0,
        TERMINATE_MAX_Z = 1,
        TERMINATE_TIME  = 2
    };
	// Sets default values for this actor's properties
	ABullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    FVector m_vVelocity;
    FVector m_vAcceleration;

    float m_fMinZ;
    float m_fMaxZ;
    float m_fMaxTime;
    int m_nTerminationMode;

    AActor* m_pOwner;

    float m_fTime;

    int m_nCollisionEnable;
};
