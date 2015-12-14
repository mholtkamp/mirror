// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "FalloutTrigger.h"
#include "Hero.h"


// Sets default values
AFalloutTrigger::AFalloutTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));
    m_pBox->InitBoxExtent(FVector(1000.0f, 400.0f, 100.0f));

    RootComponent = m_pBox;

    OnActorBeginOverlap.AddDynamic(this, &AFalloutTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFalloutTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFalloutTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AFalloutTrigger::OnOverlapBegin(AActor* pOther)
{
    AHero* pHero = Cast<AHero>(pOther);

    if (pHero != 0)
    {
        pHero->Kill();
    }
}