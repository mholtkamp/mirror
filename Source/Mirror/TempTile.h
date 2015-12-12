// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TempTile.generated.h"

UCLASS()
class MIRROR_API ATempTile : public AActor
{
	GENERATED_BODY()
	
public:	

    enum ETileState
    {
        STATE_VISIBLE = 0,
        STATE_HIDDEN = 1
    };
	// Sets default values for this actor's properties
	ATempTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void SetHidden(int nState);

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    UFUNCTION()
    void OnOverlapEnd(AActor* pOther);

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* m_pParticle;

    float m_fDisappearTime;
    float m_fAppearTime;
	int m_nActive;
    int m_nState;
	
};
