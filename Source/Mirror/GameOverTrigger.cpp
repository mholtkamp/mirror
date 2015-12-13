// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "GameOverTrigger.h"
#include "Hero.h"


// Sets default values
AGameOverTrigger::AGameOverTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(400.0f, 200.0f, 400.0f));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));

    RootComponent = m_pBox;

    OnActorBeginOverlap.AddDynamic(this, &AGameOverTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGameOverTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameOverTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGameOverTrigger::OnOverlapBegin(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        Cast<AHero>(pOther)->SetGameOver();
    }
}