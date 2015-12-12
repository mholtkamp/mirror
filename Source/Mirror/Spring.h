// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spring.generated.h"

UCLASS()
class MIRROR_API ASpring : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpring();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

	UStaticMeshComponent* m_pMesh;

    float m_pBoostSpeed;
	
};
