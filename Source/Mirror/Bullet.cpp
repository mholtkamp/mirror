// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Bullet.h"
#include "Hero.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("OverlapAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Enemies/Bomber/bullet.bullet'"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
    }

    m_vVelocity     = FVector(0.0f, 0.0f, 0.0f);
    m_vAcceleration = FVector(0.0f, 0.0f, 0.0f);

    m_fMinZ = -99999.0f;
    m_fMaxZ = 99999.0f;
    m_fTime = 0.0f;
    m_fMaxTime = 10.0f;
    m_pOwner = 0;

    m_nTerminationMode = TERMINATE_MIN_Z;

    OnActorBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ABullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTime += DeltaTime;

    m_vVelocity += m_vAcceleration * DeltaTime;
    
    FVector vNewLoc = GetActorLocation();
    vNewLoc += m_vVelocity * DeltaTime;
    SetActorLocation(vNewLoc);

    switch (m_nTerminationMode)
    {
    case TERMINATE_MIN_Z:
        if (vNewLoc.Z < m_fMinZ)
        {
            Destroy();
        }
        break;
    case TERMINATE_MAX_Z:
        if (vNewLoc.Z > m_fMaxZ)
        {
            Destroy();
        }
        break;
    case TERMINATE_TIME:
        if (m_fTime > m_fMaxTime)
        {
            Destroy();
        }
        break;
    default:
        break;
    }

}

void ABullet::OnOverlapBegin(AActor* pOther)
{
    AHero* pHero = Cast<AHero>(pOther);

    if (pHero != 0)
    {
        pHero->Kill();
    }

    if (pOther != m_pOwner && 
        Cast<ABullet>(pOther) == 0 &&
        !IsPendingKill())
    {
        Destroy();
    }
}
