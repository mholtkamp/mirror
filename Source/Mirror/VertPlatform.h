// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VertPlatform.generated.h"

UCLASS()
class MIRROR_API AVertPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVertPlatform();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

	UFUNCTION()
	void OnOverlapBegin(AActor* pOther);

	UFUNCTION()
	void OnOverlapEnd(AActor* pOther);
	
    FVector m_vCenter;

    float m_fDir;

    AActor* m_pHero;
};
