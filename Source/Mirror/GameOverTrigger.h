// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameOverTrigger.generated.h"

UCLASS()
class MIRROR_API AGameOverTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOverTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

	UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;
	
};
