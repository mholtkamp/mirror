// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "HeroMovementComponent.h"

void UHeroMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Make sure that everything is still valid, and that we are allowed to move.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * m_fSpeed;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }

    if (m_pMesh != 0 &&
        !DesiredMovementThisFrame.IsNearlyZero())
    {
        UStaticMeshComponent* pMeshComp;
        FVector vec = DesiredMovementThisFrame;
        vec.Z = 0.0f;

        float fMagnitude = vec.Size();
        float fAngle = FMath::RadiansToDegrees(FMath::Asin(vec.X/fMagnitude));

        if (fAngle > 0)
        {
            if (vec.Y < 0.0f)
            {
                fAngle = 180.0f - fAngle;
            }
        }
        else
        {
            if (vec.Y < 0.0f)
            {
                fAngle = -180.0f - fAngle;
            }
        }

        pMeshComp = reinterpret_cast<UStaticMeshComponent*>(m_pMesh);

        FRotator rot;
        rot.Pitch = 0.0f;
        rot.Roll = 0.0f;

        rot.Yaw = -1 * fAngle;

        pMeshComp->SetWorldRotation(rot);
    }
};



