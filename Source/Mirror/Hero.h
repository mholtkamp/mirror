// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Hero.generated.h"

UCLASS()
class MIRROR_API AHero : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHero();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void Jump();

    void MoveRight(float fAxisValue);

    void SetVelocity(FVector vNewVel);

    void SetMirrorMode(int nMirror);

    void Kill();

    enum EAnimState
    {
        ANIM_IDLE = 0,
        ANIM_JUMP = 1,
        ANIM_RUN  = 2,
        ANIM_FALL = 3
    };

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UCameraComponent* m_pCamera;

    UPROPERTY(EditAnywhere)
    USpringArmComponent* m_pSpringArm;

    FVector m_vVelocity;
    FVector m_vGravity;

    int m_nGrounded;
    int m_nMoving;

    int m_nMirrorMode;

    EAnimState m_nAnimState;

    class UHeroMovementComponent* m_pMovementComponent;
	
};
