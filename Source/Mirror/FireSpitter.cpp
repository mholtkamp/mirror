// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "FireSpitter.h"
#include "Bullet.h"
#include "Hero.h"
#define DEFAULT_BULLET_SPEED 2500.0f;
#define DEFAULT_FIRE_TIME 3.0f;
#define DEFAULT_ACCELERATION_SPEED 2500.0f;

// Sets default values
AFireSpitter::AFireSpitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Enemies/FireSpitter/fireSpitter.fireSpitter'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ofParticle(TEXT("ParticleSystem'/Game/StaticMeshes/Enemies/FireSpitter/p_fireSpitter.p_fireSpitter'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("OverlapAll"));

    m_pParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    m_pParticle->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        m_pParticle->SetTemplate(ofParticle.Object);
    }

    RootComponent = m_pMesh;
    m_pParticle->AttachTo(RootComponent);

    m_fBulletSpeed = DEFAULT_BULLET_SPEED;
    m_fFireTime = 0.0f;
    m_fFireRate = DEFAULT_FIRE_TIME;
    m_fAccelerationSpeed = DEFAULT_ACCELERATION_SPEED;

    OnActorBeginOverlap.AddDynamic(this, &AFireSpitter::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFireSpitter::BeginPlay()
{
	Super::BeginPlay();

    FVector m_vOrigin;
	
}

// Called every frame
void AFireSpitter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fFireTime -= DeltaTime;


    if (m_fFireTime <= 0)
    {
        m_fFireTime = m_fFireRate;
        FireBullet();
    }

}

void AFireSpitter::FireBullet()
{
    FVector vSpawnLoc = GetActorLocation();

    ABullet* pBullet = GetWorld()->SpawnActor<ABullet>();

    // Set bullet stats
    pBullet->m_nTerminationMode = ABullet::TERMINATE_MIN_Z;
    pBullet->m_vVelocity = FVector(0.0f, 0.0f, 1.0f * m_fBulletSpeed);
    pBullet->m_vAcceleration = FVector(0.0f, 0.0f, -1.0f * m_fAccelerationSpeed);
    pBullet->m_fMinZ = vSpawnLoc.Z - 10.f;
    pBullet->m_pOwner = Cast<AActor>(this);

    // Set bullet location
    pBullet->SetActorLocation(vSpawnLoc);

    // Update the transforms and overlap so it doesnt register at (0,0,0).
    pBullet->UpdateComponentTransforms();
    pBullet->UpdateOverlaps();

    pBullet->m_nCollisionEnable = 1;
}

void AFireSpitter::OnOverlapBegin(AActor* pOther)
{
    AHero* pHero = Cast<AHero>(pOther);

    if (pHero != 0)
    {
        pHero->Kill();
    }
}

