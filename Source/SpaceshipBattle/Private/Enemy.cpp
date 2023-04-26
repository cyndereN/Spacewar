// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
#include "Kismet/KismetMathLibrary.h"
#include <SpaceShipGameMode.h>
#include <EnemySpawn.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	// RootComponent = RootSceneComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetSphereRadius(100);
	RootSceneComp = CollisionComp;

	EnemySM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemySM"));
	EnemySM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemySM->SetRelativeRotation(FRotator(0, 270, 0));
	EnemySM->SetRelativeScale3D(FVector(0.75, 0.75, 0.75));
	EnemySM->SetupAttachment(RootSceneComp);

	Init();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));

	SpaceShipGameMode = Cast<ASpaceShipGameMode>(UGameplayStatics::GetGameMode(this));

	TArray<AActor *> SpawnEnemyArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawn::StaticClass(), SpawnEnemyArray);
	EnemySpawn = Cast<AEnemySpawn>(SpawnEnemyArray[0]);

	SetColor();

}

void AEnemy::Init()
{
	Speed = 0.0f;
}



// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnDeath()
{
	
	EnemySpawn->DecreaseEnemyCount();
	// Explosion effect
	// if (ExplosionParticle)
	// {
	// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	// }
	
	SpawnExplosion();
	Destroy();
}

