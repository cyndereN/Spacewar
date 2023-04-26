// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpaceShip.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "TimerManager.h"


// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	RootComponent = RootSceneComp;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	
	Init();

	GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &AEnemySpawn::SpawnEnemy, SpawnInterval, true, 0.0f);
}

void AEnemySpawn::Init()
{
	// Default distance to player
	MinimumDistanceToPlayer = 1200.0;

	// Initialization
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Default spawn interval
	SpawnInterval = 10.0f;

	MaxEnemyNum = 1;

	CurrentEnemyCount = 0;
}

FVector AEnemySpawn::GetGenerateLocation()
{
	float Distance = 0;
	FVector Location;
	while (Distance < MinimumDistanceToPlayer)
	{
		Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
		Distance = (Location - SpaceShip->GetActorLocation()).Size();
	}
	return Location;
}

void AEnemySpawn::SpawnEnemy()
{
	if (Enemy && !SpaceShip->GetBDead() && CurrentEnemyCount < MaxEnemyNum)
	{
		FVector SpawnLoation = GetGenerateLocation();
		// DrawDebugSphere(GetWorld(), SpawnLoation, 50.0f, 1, FColor::Red, false, 100);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AEnemy>(Enemy, SpawnLoation, FRotator::ZeroRotator, SpawnParams);
		CurrentEnemyCount++;
	}
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawn::DecreaseEnemyCount()
{
	if (CurrentEnemyCount > 0)
	{
		CurrentEnemyCount--;
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(CurrentEnemyCount));
	}
}

