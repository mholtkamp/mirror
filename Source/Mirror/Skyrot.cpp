// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Skyrot.h"

#define ROTATION_RATE .005f

static UMaterial* s_pRealMat = 0;
static UMaterial* s_pMirrorMat = 0;

// Sets default values
ASkyrot::ASkyrot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Skysphere/skysphere.skysphere'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofRealMat(TEXT("Material'/Game/StaticMeshes/Skysphere/M_Skysphere_Real.M_Skysphere_Real'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMirrorMat(TEXT("Material'/Game/StaticMeshes/Skysphere/M_Skysphere_Mirror.M_Skysphere_Mirror'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    if (ofMesh.Succeeded()      &&
        ofRealMat.Succeeded()   &&
        ofMirrorMat.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        s_pRealMat = ofRealMat.Object;
        s_pMirrorMat = ofMirrorMat.Object;
    }

    RootComponent = m_pMesh;
}

// Called when the game starts or when spawned
void ASkyrot::BeginPlay()
{
	Super::BeginPlay();

    m_pHero = UGameplayStatics::GetPlayerController(this, 0)->GetPawn();

    if (m_pHero == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Hero is unset in skyrot"));
    }
	
}

// Called every frame
void ASkyrot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if (m_pHero != 0)
    {
        FVector vNewLoc = GetActorLocation();
        vNewLoc.X = m_pHero->GetActorLocation().X;
        SetActorLocation(vNewLoc);
    }

    SetActorRotation(FRotator(0.0f, -1.0f * m_pHero->GetActorLocation().X * ROTATION_RATE, 0.0f));
}

void ASkyrot::SetMirrorMode(int nMirror)
{
    if (nMirror == 0)
    {
        m_pMesh->SetMaterial(0, s_pRealMat);
    }
    else
    {
        m_pMesh->SetMaterial(0, s_pMirrorMat);
    }
}
