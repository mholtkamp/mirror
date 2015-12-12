// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "ModeShifter.h"
#include "Hero.h"


// Sets default values
AModeShifter::AModeShifter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));

    RootComponent = m_pBox;

    OnActorBeginOverlap.AddDynamic(this, &AModeShifter::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AModeShifter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AModeShifter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AModeShifter::OnOverlapBegin(AActor* pOther)
{
    if (Cast<AHero>(pOther) != 0)
    {
        // Set mirror flag

        // Swap material on skyrot
        if (m_pSkyrot != 0)
        {
            m_pSkyrot->SetMirrorMode(static_cast<int>(m_bMirror));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Skysphere was not assigned to ModeShifter"));
        }
    }
}
