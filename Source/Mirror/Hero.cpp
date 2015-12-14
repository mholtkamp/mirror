// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "Hero.h"
#include "HeroMovementComponent.h"
#include "Skyrot.h"

#define DEAD_ZONE 0.2f
#define CAMERA_ROT_SPEED 200.0f
#define SPRINGARM_PITCH -5.0f
#define SPRINGARM_LENGTH 1300.0f
#define GROUNDED_CHECK_DISTANCE 20.0f
#define HERO_GRAVITY -3000.0f
#define DEFAULT_HERO_SPEED 1000.0f;

#define DEATH_FADE_OUT_TIME 1.0f
#define DEATH_FADE_PAUSE_TIME 2.0f
#define DEATH_FADE_IN_TIME 2.5f
#define DEATH_TIME_END 3.0f

#define GAME_OVER_SCREEN_TIME 2.0f
#define GAME_OVER_ZOOM_SPEED 300.0f

static const FVector SPAWN_LOC(-238.140808f, 0.0f, 2213.46435f);
static const FVector MIRROR_SPAWN_LOC(39140.4218f, 0.0f, -6605.53027f);

static UAnimSequence* s_pAnimIdle;
static UAnimSequence* s_pAnimRun;
static UAnimSequence* s_pAnimJump;
static UAnimSequence* s_pAnimFall;
static UAnimSequence* s_pAnimFlap;

static USoundWave* s_pRealSong   = 0;
static USoundWave* s_pMirrorSong = 0;

static UMaterial* s_pRealMat   = 0;
static UMaterial* s_pMirrorMat = 0;

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

    static ConstructorHelpers::FObjectFinder<UMaterial> ofRealMat(TEXT("Material'/Game/SkeletalMeshes/Captain/CaptainMat.CaptainMat'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMirrorMat(TEXT("Material'/Game/SkeletalMeshes/Captain/M_Captain_Mirror.M_Captain_Mirror'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ofParticle(TEXT("ParticleSystem'/Game/Particles/Poof.Poof'"));

    static ConstructorHelpers::FObjectFinder<USoundWave> ofRealSong(TEXT("SoundWave'/Game/Music/JungleSong16Bit.JungleSong16Bit'"));
    static ConstructorHelpers::FObjectFinder<USoundWave> ofMirrorSong(TEXT("SoundWave'/Game/Music/MirrorSong.MirrorSong'"));

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(30.0f, 30.0f, 100.0f));
    m_pBox->SetCollisionProfileName(TEXT("Pawn"));

    m_pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    m_pDeathParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Particle"));
    m_pDeathParticle->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

    m_pAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetSkeletalMesh(ofMesh.Object);
        m_pMesh->SetWorldScale3D(FVector(0.75f, 0.75f, 0.75f));
        m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

        m_pDeathParticle->SetTemplate(ofParticle.Object);

        s_pAnimIdle = ofAnimIdle.Object;
        s_pAnimRun  = ofAnimRun.Object;
        s_pAnimJump = ofAnimJump.Object;
        s_pAnimFall = ofAnimFall.Object;
        s_pAnimFlap = ofAnimFlap.Object;

        s_pRealMat   = ofRealMat.Object;
        s_pMirrorMat = ofMirrorMat.Object;

        s_pRealSong = ofRealSong.Object;
        s_pMirrorSong = ofMirrorSong.Object;
    }

    m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    m_pSpringArm->TargetArmLength = SPRINGARM_LENGTH;
    m_pSpringArm->SetRelativeRotation(FRotator(SPRINGARM_PITCH, -90, 0.0f));
    m_pSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
    m_pSpringArm->bEnableCameraLag = 1;
    m_pSpringArm->bDoCollisionTest = 0;


    m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    m_pCamera->PostProcessBlendWeight = 1.0f;
        
    // Attach components together
    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);
    m_pSpringArm->AttachTo(RootComponent);
    m_pCamera->AttachTo(m_pSpringArm);
    m_pDeathParticle->AttachTo(RootComponent);
    m_pAudioComponent->AttachTo(RootComponent);

    m_vVelocity = FVector::ZeroVector;
    m_vGravity = FVector(0.0f, 0.0f, HERO_GRAVITY);
    
    m_nAnimState = ANIM_IDLE;

    m_pMovementComponent = CreateDefaultSubobject<UHeroMovementComponent>(TEXT("Movement Component"));
    m_pMovementComponent->UpdatedComponent = RootComponent;
    m_pMovementComponent->m_pMesh = m_pMesh;
    m_pMovementComponent->m_fSpeed = DEFAULT_HERO_SPEED;
    m_pMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
    m_pMovementComponent->bConstrainToPlane = 1;

    m_nGrounded   = 0;
    m_nMoving     = 0;
    m_nMirrorMode = 0;
    m_nAlive      = 1;

    m_pSkyrot = 0;
    m_nGameOver = 0;
    m_fGameOverTime = 0.0f;
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
	
    m_pMesh->PlayAnimation(s_pAnimIdle, 1);
    m_pDeathParticle->Deactivate();

    for (TActorIterator<ASkyrot> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	{
		m_pSkyrot = *ActorItr;
	}

    m_pAudioComponent->Sound = s_pRealSong;
    m_pAudioComponent->Play();
}

// Called every frame
void AHero::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if (m_nAlive == 0)
    {
        //  Update death sequence
        UpdateDeathSequence(DeltaTime);
        return;
    }

    if (m_nGameOver)
    {
        UpdateGameOverSequence(DeltaTime);
        return;
    }

    FVector vNewPos = GetActorLocation();

    // Apply gravity
    m_vVelocity += m_vGravity * DeltaTime;

    // Move along z axis
    FHitResult hit;
    vNewPos = GetActorLocation();
    vNewPos.Z += m_vVelocity.Z * DeltaTime;
    SetActorLocation(vNewPos, 1, &hit);

    if (hit.Actor != 0)
    {
        m_vVelocity.Z = 0.0f;
        m_nGrounded = 1;
    }
    else
    {
        // if on previous frame, was grounded
        if (m_nGrounded)
        {
            FVector vCurLoc = GetActorLocation();
            vNewPos = vCurLoc;
            vNewPos.Z -= 20.0f;
            SetActorLocation(vNewPos, 1, &hit);
            
            if (hit.Actor != 0)
            {
                // Redundant, maybe remove.
                m_nGrounded = 1;   
            }
            else
            {
                SetActorLocation(vCurLoc, 0);
                m_nGrounded = 0;
            }
        }
        else
        {
            m_nGrounded = 0;
        }
        
    }

    // Update animations
    if (m_nAnimState == ANIM_JUMP)
    {
        if (m_nGrounded)
        {
            m_pMesh->PlayAnimation(s_pAnimIdle, 1);
            m_nAnimState = ANIM_IDLE;
        }

        if (!m_pMesh->IsPlaying())
        {
            m_pMesh->PlayAnimation(s_pAnimFall, 1);
            m_nAnimState = ANIM_FALL;
        }
    }
    else if (m_nAnimState == ANIM_FALL)
    {
        if (m_nGrounded)
        {
            m_pMesh->PlayAnimation(s_pAnimIdle, 1);
            m_nAnimState = ANIM_IDLE;
        }
    }
    
    if (m_nAnimState == ANIM_IDLE)
    {
        if (m_nMoving)
        {
            m_pMesh->PlayAnimation(s_pAnimRun, 1);
            m_nAnimState = ANIM_RUN;
        }
    }

    if (m_nAnimState == ANIM_RUN)
    {
        if (!m_nMoving)
        {
            m_pMesh->PlayAnimation(s_pAnimIdle, 1);
            m_nAnimState = ANIM_IDLE;
        }
    }

    UpdateComponentTransforms();
    UpdateOverlaps();
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
    if (m_nGrounded &&
        m_nAlive    &&
        !m_nGameOver)
    {
        m_vVelocity.Z = 2000.0f;

        m_pMesh->SetAnimation(s_pAnimFall);
        m_pMesh->PlayAnimation(s_pAnimJump, 0);

        m_nAnimState = ANIM_JUMP;
        m_nGrounded = 0;
    }
    else if (m_nGameOver &&
             m_fGameOverTime > GAME_OVER_SCREEN_TIME)
    {
        UGameplayStatics::OpenLevel(this, TEXT("MenuMap"));
    }
}

void AHero::MoveRight(float fAxisValue)
{
    if (m_pMovementComponent && (m_pMovementComponent->UpdatedComponent == RootComponent) && (FMath::Abs(fAxisValue) > DEAD_ZONE) && m_nAlive && !m_nGameOver)
    {
        m_pMovementComponent->AddInputVector(FVector(1.0f, 0.0f, 0.0f) * fAxisValue);
    }

    if ((FMath::Abs(fAxisValue) > DEAD_ZONE)
        && m_nAlive
        && !m_nGameOver)
    {
        m_nMoving = 1;
    }
    else
    {
        m_nMoving = 0;
    }
}

void AHero::SetVelocity(FVector vNewVel)
{
    m_vVelocity = vNewVel;
}

void AHero::SetMirrorMode(int nMirror)
{
    if (nMirror != 0)
    {
        m_nMirrorMode = 1;
    }

    if (nMirror == 0)
    {
        m_pMesh->SetMaterial(0, s_pRealMat);

        // Don't restart the real song if its already playing
        if (m_pAudioComponent->Sound != s_pRealSong)
        {
            m_pAudioComponent->Stop();
            m_pAudioComponent->Sound = s_pRealSong;
            m_pAudioComponent->Play();
        }
        
    }
    else
    {
        m_pMesh->SetMaterial(0, s_pMirrorMat);

        if (m_pAudioComponent->Sound != s_pMirrorSong)
        {
            m_pAudioComponent->Stop();
            m_pAudioComponent->Sound = s_pMirrorSong;
            m_pAudioComponent->Play();
        }
    }
}

void AHero::Kill()
{
    if (m_nAlive    != 0 &&
        m_nGameOver == 0)
    {
        m_nAlive = 0;
        m_fDeathTime = 0.0f;
        m_pDeathParticle->Activate();
        m_pMesh->SetHiddenInGame(1);

        m_pMesh->PlayAnimation(s_pAnimIdle, 1);
        m_nAnimState = ANIM_IDLE;
    }
}

void AHero::UpdateDeathSequence(float fDeltaTime)
{
    float fIntensity = 0.0f;

    // Pre-fade state. Only particle effect is visible
    if (m_fDeathTime < DEATH_FADE_OUT_TIME)
    {
        m_fDeathTime += fDeltaTime;
        if (m_fDeathTime >= DEATH_FADE_OUT_TIME)
        {
            m_pDeathParticle->Deactivate();
            m_pCamera->PostProcessSettings.bOverride_SceneColorTint = 1;
            m_pCamera->PostProcessSettings.bOverride_BloomIntensity = 1;
        }
    }

    // Fade out state, fades scene color to black
    else if (m_fDeathTime < DEATH_FADE_PAUSE_TIME)
    {
        m_fDeathTime += fDeltaTime;

        fIntensity = (DEATH_FADE_PAUSE_TIME - m_fDeathTime)/(DEATH_FADE_PAUSE_TIME - DEATH_FADE_OUT_TIME);
        m_pCamera->PostProcessSettings.SceneColorTint = FLinearColor(fIntensity, fIntensity, fIntensity);
        m_pCamera->PostProcessSettings.BloomIntensity = fIntensity;

        if (m_fDeathTime > DEATH_FADE_PAUSE_TIME)
        {
            fIntensity = 0.0f;
            m_pCamera->PostProcessSettings.SceneColorTint = FLinearColor(fIntensity, fIntensity, fIntensity);
            m_pCamera->PostProcessSettings.BloomIntensity = fIntensity;
            m_pMesh->SetHiddenInGame(0);
            Spawn();
        }
    }

    // Pause state, screen stays black
    else if (m_fDeathTime < DEATH_FADE_IN_TIME)
    {
        m_fDeathTime += fDeltaTime;

    }

    // Fade in state, camera fades into normal scene color
    else if (m_fDeathTime < DEATH_TIME_END)
    {
        m_fDeathTime += fDeltaTime;
        fIntensity = 1.0f - (DEATH_TIME_END - m_fDeathTime)/(DEATH_TIME_END - DEATH_FADE_IN_TIME);
        m_pCamera->PostProcessSettings.SceneColorTint = FLinearColor(fIntensity, fIntensity, fIntensity);
        m_pCamera->PostProcessSettings.BloomIntensity = fIntensity;
    }

    // Death sequence over
    else
    {
        m_fDeathTime += fDeltaTime;
        m_pCamera->PostProcessSettings.bOverride_SceneColorTint = 0;
        m_pCamera->PostProcessSettings.bOverride_BloomIntensity = 0;
        m_nAlive = 1;
    }

}

void AHero::Spawn()
{
    m_nGrounded = 1;
    m_nMoving = 0;
    m_vVelocity = FVector(0.0f, 0.0f, 0.0f);
    
    m_pMesh->PlayAnimation(s_pAnimIdle, 1);
    m_nAnimState = ANIM_IDLE;

    if (m_nMirrorMode == 0)
    {
        SetActorLocation(SPAWN_LOC);
    }
    else
    {
        SetActorLocation(MIRROR_SPAWN_LOC);
    }

    // Update the skyrot material.
    if (Cast<ASkyrot>(m_pSkyrot) != 0)
    {
        Cast<ASkyrot>(m_pSkyrot)->SetMirrorMode(0);
    }
    // Update hero material
    SetMirrorMode(0);
}

void AHero::SetGameOver()
{
    m_nGameOver = 1;
    m_fGameOverTime = 0.0f;
    m_nMoving = 0;

    m_pMesh->PlayAnimation(s_pAnimIdle, 1);
    m_nAnimState = ANIM_IDLE;
}

void AHero::UpdateGameOverSequence(float fDeltaTime)
{
     // Apply gravity
    m_vVelocity += m_vGravity * fDeltaTime;

    // Move along z axis
    FHitResult hit;
    FVector vNewPos = GetActorLocation();
    vNewPos.Z += m_vVelocity.Z * fDeltaTime;
    SetActorLocation(vNewPos, 1, &hit);

    if (hit.Actor != 0)
    {
        m_vVelocity.Z = 0.0f;
        m_nGrounded = 1;
    }

    if (m_fGameOverTime < GAME_OVER_SCREEN_TIME)
    {
        m_fGameOverTime += fDeltaTime;
        m_pSpringArm->TargetArmLength = m_pSpringArm->TargetArmLength - fDeltaTime * GAME_OVER_ZOOM_SPEED;
    }
}