// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "FireSpitter.h"
#define SPAWN_RATE 5.0f 


// Sets default values
AFireSpitter::AFireSpitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Enemies/FireSpitter/fireSpitter.fireSpitter'"));
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
    }

    RootComponent = m_pMesh;

    m_fCountdown = SPAWN_RATE;

}

// Called when the game starts or when spawned
void AFireSpitter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireSpitter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fCountdown -= DeltaTime;

    if (m_fCountdown <= 0)
    {
        // Spawn Bullet
        m_fCountdown = SPAWN_RATE;
    }

}

