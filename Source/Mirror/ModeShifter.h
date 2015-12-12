// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Skyrot.h"
#include "ModeShifter.generated.h"


UCLASS()
class MIRROR_API AModeShifter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModeShifter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    UPROPERTY(EditAnywhere)
	bool m_bMirror;
	
    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    UPROPERTY(EditAnywhere)
    ASkyrot* m_pSkyrot;
};
