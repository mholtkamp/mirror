// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "TempTile.h"
#include "Hero.h"

#define DISAPPEAR_TIME 0.4f
#define APPEAR_TIME 5.0f

// Sets default values
ATempTile::ATempTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));

    m_pBox->InitBoxExtent(FVector(160.0f, 132.0f, 32.0f));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));
    m_pMesh->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Platforms/Tile/tile.tile'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMaterial(TEXT("Material'/Game/StaticMeshes/Platforms/Tile/M_Tile_Temp.M_Tile_Temp'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ofParticle(TEXT("ParticleSystem'/Game/Particles/Poof.Poof'"));

    if (ofMesh.Succeeded() &&
        ofMaterial.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        m_pMesh->SetMaterial(0, ofMaterial.Object);
        m_pParticle->SetTemplate(ofParticle.Object);
    }
    
    m_fDisappearTime = DISAPPEAR_TIME;
    m_fAppearTime    = APPEAR_TIME;
    m_nActive        = 0;
    m_nState = STATE_VISIBLE;

    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);
    m_pParticle->AttachTo(RootComponent);

    OnActorBeginOverlap.AddDynamic(this, &ATempTile::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ATempTile::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATempTile::BeginPlay()
{
	Super::BeginPlay();

    m_pParticle->SetHiddenInGame(1);
	
}

// Called every frame
void ATempTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if (m_nState  == STATE_VISIBLE &&
        m_nActive != 0)
    {
        m_fDisappearTime -= DeltaTime;

        if (m_fDisappearTime < 0.0f)
        {
            // Make tile disappear.
            SetHidden(STATE_HIDDEN);
        }
    }
    else if (m_nState == STATE_HIDDEN)
    {
        m_fAppearTime -= DeltaTime;

        if (m_fAppearTime <= 0.0f)
        {
            // Make tile appear again.
            SetHidden(STATE_VISIBLE);
        }
    }

}

void ATempTile::OnOverlapBegin(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        m_nActive = 1;
    }
    
}

void ATempTile::OnOverlapEnd(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        m_fDisappearTime = DISAPPEAR_TIME;
    }
}

void ATempTile::SetHidden(int nState)
{
    if (nState == STATE_HIDDEN)
    {
        m_pMesh->SetCollisionProfileName(TEXT("NoCollision"));
        m_pMesh->SetHiddenInGame(1);
        m_pParticle->SetHiddenInGame(0);
        m_pParticle->Activate();
        m_fAppearTime = APPEAR_TIME;
    }
    else
    {
        m_pMesh->SetCollisionProfileName(TEXT("BlockAll"));
        m_pMesh->SetHiddenInGame(0);
        m_pParticle->Deactivate();
        m_fDisappearTime = DISAPPEAR_TIME;
    }

    m_nActive = 0;
    m_nState = nState;
}