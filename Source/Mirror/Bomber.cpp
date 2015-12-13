// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Bomber.h"

#define DEFAULT_SPEED 300.0f
#define DEFAULT_BULLET_SPEED 500.0f
#define DEFAULT_RANGE 1300.0f
#define DEFAULT_FIRE_TIME 0.7f



// Sets default values
ABomber::ABomber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Enemies/Bomber/bomber.bomber'"));
    
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("OverlapAll"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
    }
    
    m_fSpeed       = DEFAULT_SPEED;
    m_fBulletSpeed = DEFAULT_BULLET_SPEED;
    m_fRange       = DEFAULT_RANGE;
    m_fFireTime    = DEFAULT_FIRE_TIME;

    m_fDirection = 1.0f;
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


}

