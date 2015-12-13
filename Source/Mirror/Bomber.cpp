// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Bomber.h"
#include "Bullet.h"
#include "Hero.h"

#define DEFAULT_SPEED 500.0f
#define DEFAULT_BULLET_SPEED 1000.0f
#define DEFAULT_RANGE 1000.0f
#define DEFAULT_FIRE_TIME 2.0f

#define DEFAULT_BULLET_LIFETIME 5.0f



// Sets default values
ABomber::ABomber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Enemies/Bomber/bomber.bomber'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ofParticle(TEXT("ParticleSystem'/Game/StaticMeshes/Enemies/Bomber/P_Bomber.P_Bomber'"));
    
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("OverlapAll"));

    m_pParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    m_pParticle->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));


    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        m_pParticle->SetTemplate(ofParticle.Object);
    }
    
    m_fSpeed          = DEFAULT_SPEED;
    m_fBulletSpeed    = DEFAULT_BULLET_SPEED;
    m_fRange          = DEFAULT_RANGE;
    m_fFireTime       = DEFAULT_FIRE_TIME;
    m_fBulletLifetime = DEFAULT_BULLET_LIFETIME;

    m_fDirection = 1.0f;

    RootComponent = m_pMesh;
    m_pParticle->AttachTo(RootComponent);

    OnActorBeginOverlap.AddDynamic(this, &ABomber::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABomber::BeginPlay()
{
	Super::BeginPlay();

    m_vOrigin = GetActorLocation();
	
}

// Called every frame
void ABomber::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTime += DeltaTime;

    FVector vNewLoc = GetActorLocation();
    vNewLoc.X += m_fDirection * m_fSpeed * DeltaTime;
    SetActorLocation(vNewLoc);

    if (vNewLoc.X > m_vOrigin.X + m_fRange)
    {
        m_fDirection = -1.0f;
    }
    else if (vNewLoc.X < m_vOrigin.X - m_fRange)
    {
        m_fDirection = 1.0f;
    }

    if (m_fTime >= m_fFireTime)
    {
        m_fTime = 0.0f;
        FireBullet();
    }

}

void ABomber::FireBullet()
{
    FVector vSpawnLoc = GetActorLocation();

    ABullet* pBullet = GetWorld()->SpawnActor<ABullet>();
    pBullet->SetActorLocation(vSpawnLoc);

    // Update the transforms and overlap so it doesnt register at (0,0,0).
    UpdateComponentTransforms();
    UpdateOverlaps();

    // Set bullet stats
    pBullet->m_nTerminationMode = ABullet::TERMINATE_TIME;
    pBullet->m_fMaxTime = m_fBulletLifetime;
    pBullet->m_vVelocity = FVector(0.0f, 0.0f, -1.0f * m_fBulletSpeed);    
}

void ABomber::OnOverlapBegin(AActor* pOther)
{
    AHero* pHero = Cast<AHero>(pOther);

    if (pHero != 0)
    {
        pHero->Kill();
    }
}