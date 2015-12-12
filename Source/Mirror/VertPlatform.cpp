// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "VertPlatform.h"
#include "Hero.h"

// Sets default values
AVertPlatform::AVertPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Platforms/Cloud/cloud.cloud'"));
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pMesh->SetCollisionProfileName(TEXT("BlockAll"));
    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));

	if (ofMesh.Succeeded())
	{
		m_pMesh->SetStaticMesh(ofMesh.Object);
	}

	RootComponent = m_pMesh;
    m_pBox->AttachTo(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &AVertPlatform::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AVertPlatform::OnOverlapEnd);

    m_fDir = 1.0f;

    m_pHero = 0;
}

// Called when the game starts or when spawned
void AVertPlatform::BeginPlay()
{
	Super::BeginPlay();

    m_vCenter = GetActorLocation();

	
}

// Called every frame
void AVertPlatform::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    FVector vCurrentPos = GetActorLocation();
 
    if (vCurrentPos.Z - m_vCenter.Z > 500 )
    {
        m_fDir = -1.0f;
        // SetActorLocation(FVector(-10.0f, 0.0f, 0.0f), 0);
    }
    if (vCurrentPos.Z - m_vCenter.Z <= 0)
    {
        m_fDir = 1.0f;
    }

    FVector vNewPos = vCurrentPos + FVector(0.0f, 0.0f, 130.0f)*m_fDir*DeltaTime;

    SetActorLocation(vNewPos, 0);

    if (m_pHero != 0)
    {
        vNewPos = m_pHero->GetActorLocation() + FVector(0.0f, 0.0f, 130.0f)*m_fDir*DeltaTime;
        m_pHero->SetActorLocation(vNewPos);
        m_pHero->UpdateComponentTransforms();
        m_pHero->UpdateOverlaps();

    }


}

void AVertPlatform::OnOverlapBegin(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        m_pHero = pOther;
    }


}

void AVertPlatform::OnOverlapEnd(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        m_pHero = 0;
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Off the Cloud");
    }
    
}

