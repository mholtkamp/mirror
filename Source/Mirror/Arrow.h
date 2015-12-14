// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class MIRROR_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	float m_fTime;

    UPROPERTY(EditAnywhere)
    bool m_bPointDown;

	UPROPERTY(EditAnywhere)
    FVector m_vColor;

    UPROPERTY(EditAnywhere)
    float m_fPulseTime;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UMaterialInstanceDynamic* m_pMat;
};
