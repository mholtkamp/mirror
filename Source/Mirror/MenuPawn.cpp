// Fill out your copyright notice in the Description page of Project Settings.

#include "Mirror.h"
#include "MenuPawn.h"


// Sets default values
AMenuPawn::AMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	ACameraActor* pCamera = 0;

    for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	{
        if (ActorItr->GetName().Equals(TEXT("MainCamera")))
        {
            pCamera = *ActorItr;
            break;
        }   
	}

    UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(pCamera);
}

// Called every frame
void AMenuPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMenuPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

