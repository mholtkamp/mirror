// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Arrow.h"

#define PI_2 6.28318530718

static UMaterial* s_pParentMat = 0;

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Props/arrow.arrow'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat(TEXT("Material'/Game/StaticMeshes/Props/M_Arrow.M_Arrow'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("NoCollision"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        s_pParentMat = ofMat.Object; 
    }

    m_fTime = 0.0f;

    m_bPointDown = true;
    m_vColor = FVector(0.8f, 0.4f, 0.4f);
    m_fPulseTime =  2.0f;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();

    m_pMat = UMaterialInstanceDynamic::Create(s_pParentMat, this);
    m_pMesh->SetMaterial(0, m_pMat);
    m_pMat->SetVectorParameterValue(TEXT("ArrowColor"), m_vColor);
	
}

// Called every frame
void AArrow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTime += DeltaTime;
    
    float fAlphaVal = FMath::Sin(m_fTime * (PI_2/m_fPulseTime));
    fAlphaVal = FMath::Abs(fAlphaVal);
    fAlphaVal = FMath::Clamp(fAlphaVal, 0.4f, 1.0f);
    

    m_pMat->SetScalarParameterValue(TEXT("ArrowAlpha"), fAlphaVal);
}

