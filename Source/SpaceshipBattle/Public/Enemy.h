// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class ASpaceShip;
class ASpaceShipGameMode;
class AEnemySpawn;

UCLASS()
class SPACESHIPBATTLE_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USceneComponent *RootSceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USphereComponent *CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UStaticMeshComponent *EnemySM;

	// Particle effect of explosion
	UPROPERTY(EditAnywhere, Category = "Particle")
		UParticleSystem* ExplosionParticle;

	ASpaceShip *SpaceShip;

	ASpaceShipGameMode *SpaceShipGameMode;

	AEnemySpawn *EnemySpawn;

protected:

	void Init();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Random enemy color
	UFUNCTION(BlueprintImplementableEvent)
		void SetColor();
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnExplosion();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void OnDeath();

public:

	UPROPERTY(EditAnywhere, Category = "Default")
	float Speed;

};
