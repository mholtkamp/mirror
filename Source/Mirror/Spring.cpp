// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Spring.h"
#include "Hero.h"
#include "HeroMovementComponent.h"

// Sets default values
ASpring::ASpring()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Platforms/Spring/spring.spring'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("OverlapAll"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
    }

    RootComponent = m_pMesh;

    OnActorBeginOverlap.AddDynamic(this, &ASpring::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASpring::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpring::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ASpring::OnOverlapBegin(AActor* pOther)
{
    AHero* pHero = Cast<AHero>(pOther);

    if(pHero != 0)
    {
        FVector vExtent = pHero->m_pBox->GetScaledBoxExtent();
        if ((pHero->GetActorLocation().Z - vExtent.Z) > GetActorLocation().Z)
        {
            pHero->SetVelocity(FVector(0.0f, 0.0f, 4000.0f));
        }
    }
}
