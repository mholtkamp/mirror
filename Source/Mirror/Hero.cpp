// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Hero.h"
#include "HeroMovementComponent.h"

#define DEAD_ZONE 0.2f
#define CAMERA_ROT_SPEED 200.0f
#define SPRINGARM_PITCH -20.0f

static UAnimSequence* s_pAnimIdle;
static UAnimSequence* s_pAnimRun;
static UAnimSequence* s_pAnimJump;
static UAnimSequence* s_pAnimFall;
static UAnimSequence* s_pAnimFlap;

// Sets default values
AHero::AHero()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Find assets needed by hero class
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ofMesh(TEXT("SkeletalMesh'/Game/SkeletalMeshes/Captain/captain.captain'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofAnimIdle(TEXT("AnimSequence'/Game/SkeletalMeshes/Captain/captain_Anim_Armature_Idle.captain_Anim_Armature_Idle'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofAnimRun(TEXT("AnimSequence'/Game/SkeletalMeshes/Captain/captain_Anim_Armature_Walk.captain_Anim_Armature_Walk'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofAnimJump(TEXT("AnimSequence'/Game/SkeletalMeshes/Captain/captain_Anim_Armature_Jump.captain_Anim_Armature_Jump'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofAnimFall(TEXT("AnimSequence'/Game/SkeletalMeshes/Captain/captain_Anim_Armature_Fall.captain_Anim_Armature_Fall'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofAnimFlap(TEXT("AnimSequence'/Game/SkeletalMeshes/Captain/captain_Anim_Armature_Flap.captain_Anim_Armature_Flap'"));

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(30.0f, 30.0f, 100.0f));
    m_pBox->SetCollisionProfileName(TEXT("Pawn"));

    m_pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetSkeletalMesh(ofMesh.Object);
        m_pMesh->SetWorldScale3D(FVector(0.75f, 0.75f, 0.75f));
        m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

        s_pAnimIdle = ofAnimIdle.Object;
        s_pAnimRun  = ofAnimRun.Object;
        s_pAnimJump = ofAnimJump.Object;
        s_pAnimFall = ofAnimFall.Object;
        s_pAnimFlap = ofAnimFlap.Object;
    }

    m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    m_pSpringArm->TargetArmLength = 850.0f;
    m_pSpringArm->SetRelativeRotation(FRotator(SPRINGARM_PITCH, -90.0f, 0.0f));
    m_pSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

    m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
        
    // Attach components together
    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);
    m_pSpringArm->AttachTo(RootComponent);
    m_pCamera->AttachTo(m_pSpringArm);

    m_vVelocity = FVector::ZeroVector;
    m_vGravity = FVector(0.0f, 0.0f, -980.0f);
    
    m_animState = ANIM_IDLE;

    m_pMovementComponent = CreateDefaultSubobject<UHeroMovementComponent>(TEXT("Movement Component"));
    m_pMovementComponent->UpdatedComponent = RootComponent;
    m_pMovementComponent->m_pMesh = m_pMesh;
    m_pMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
    m_pMovementComponent->bConstrainToPlane = 1;
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
	
    m_pMesh->PlayAnimation(s_pAnimIdle, 1);
}

// Called every frame
void AHero::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    FVector vNewPos = GetActorLocation();

    // Apply gravity
    m_vVelocity += m_vGravity * DeltaTime;

    // Move along z axis
    FHitResult hit;
    vNewPos = GetActorLocation();
    vNewPos.Z += m_vVelocity.Z * DeltaTime;
    if (!SetActorLocation(vNewPos, 1, &hit))
    {
        m_vVelocity.Z = 0.0f;
    }


    // Check if jump animation has ended
    if (m_animState == ANIM_JUMP)
    {
        if (!m_pMesh->IsPlaying())
        {
            m_pMesh->PlayAnimation(s_pAnimIdle, 1);
        }
    }
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHero::Jump);
    InputComponent->BindAxis(TEXT("MoveRight"),   this, &AHero::MoveRight);
}

void AHero::Jump()
{
   m_vVelocity.Z = 1000.0f;

   m_pMesh->SetAnimation(s_pAnimFall);
   m_pMesh->PlayAnimation(s_pAnimJump, 0);

   m_animState = ANIM_JUMP;
   
}

void AHero::MoveRight(float fAxisValue)
{
    if (m_pMovementComponent && (m_pMovementComponent->UpdatedComponent == RootComponent))
    {
        m_pMovementComponent->AddInputVector(FVector(1.0f, 0.0f, 0.0f) * fAxisValue);
    }
}