// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Bullet.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"





// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	// RootComponent = RootSceneComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetSphereRadius(100);
	RootComponent = CollisionComp;

	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	// ShipSM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShipSM->SetRelativeRotation(FRotator(0, 270, 0));
	ShipSM->SetRelativeScale3D(FVector(0.75, 0.75, 0.75));
	ShipSM->SetupAttachment(RootComponent);


	SpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringComp"));
	SpringComp->SetRelativeRotation(FRotator(270,0,0));
	SpringComp->bInheritYaw = false;
	SpringComp->bInheritRoll = false;
	SpringComp->bInheritPitch = false;
	SpringComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComp->SetOrthoWidth(6000.0);
	CameraComp->SetupAttachment(SpringComp);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetRelativeLocation(FVector(0, 190, 0));
	SpawnPoint->SetRelativeRotation(FRotator(0, 90, 0));
	SpawnPoint->SetupAttachment(ShipSM);

	ThrusterParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrusterParticleComp"));
	ThrusterParticleComp->SetRelativeLocation(FVector(-80, 0, 0));
	ThrusterParticleComp->SetRelativeRotation(FRotator(0, 0, 0));
	ThrusterParticleComp->SetupAttachment(RootComponent);


	Init();

}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
	// Get controller
	PC = Cast<APlayerController>(GetController());
	// Get mouse cursor
	PC->bShowMouseCursor = true;
	//Initial MovementMode
	bIsForward = true;
	bIsRight = true;
}

void ASpaceShip::Init()
{
	Speed = 2500.0f;
	BetweenShot = 0.2;
	bDead = false;
}

void ASpaceShip::LookAtCursor()
{
	// Get cursor transform
	FVector MouseLocation, MosueDirection, TargetLocation;
	PC->DeprojectMousePositionToWorld(MouseLocation, MosueDirection);
	TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	// Get angle from one location of another
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(LookAtRotator);
}

void ASpaceShip::Move(float DeltaTime)
{
	AddActorWorldOffset(ConsumeMovementInputVector()*Speed*DeltaTime, true);
}

void ASpaceShip::MoveForward(float Value)
{
	if (Value != 0)
	{
		bForwardMove = true;
	}
	else
	{
		bForwardMove = false;
	}
	if (bIsForward)
	{
		AddMovementInput(FVector::ForwardVector, Value);
	}
}

void ASpaceShip::MoveRight(float Value)
{
	if (Value != 0)
	{
		bRightMove = true;
	}
	else
	{
		bRightMove = false;
	}

	if (bIsRight)
	{
		AddMovementInput(FVector::RightVector, Value);
	}
	
}

void ASpaceShip::Fire()
{
	/*if (Bullet && !bDead)
	{
		FActorSpawnParameters SpawnParames;
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParames);
		if (ShootCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ShootCue, GetActorLocation());
		}
	}*/
}

void ASpaceShip::StartFire()
{
	GetWorldTimerManager().SetTimer(TimerHandle_BetweenShot, this, &ASpaceShip::Fire, BetweenShot, true, 0.0);
}

void ASpaceShip::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BetweenShot);
}

void ASpaceShip::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, "MainMap");
}

void ASpaceShip::OnDeath()
{
	bDead = true;
	CollisionComp->SetVisibility(false, true);
	if (GameOverCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GameOverCue, GetActorLocation());
	}
	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	GetWorldTimerManager().SetTimer(TimerHandle_Restart, this, &ASpaceShip::RestartLevel, 2.0f, false);
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDead)
	{
		if (bForwardMove || bRightMove)
		{
			ThrusterParticleComp->Activate();
		}
		else
		{
			ThrusterParticleComp->Deactivate();
		}
		LookAtCursor();
		Move(DeltaTime);
	}
	else
	{
		ThrusterParticleComp->Deactivate();
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceShip::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShip::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShip::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceShip::EndFire);
}

void ASpaceShip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*Super::NotifyActorBeginOverlap(OtherActor);

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		
		Enemy->OnDeath();
	}*/
}

